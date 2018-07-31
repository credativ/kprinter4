/* KRPINTER4 - Simple PostScript document printer
 * Copyright (C) 2014 Marco Nelles, credativ GmbH (marco.nelles@credativ.de)
 * <http://www.credativ.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QPainter>
#include <QDebug>
#include <QTranslator>
#include <QLibraryInfo>
#include <QMutex>
#include <QTextStream>

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KDEPrintDialog>
#include <KPrintPreview>
#include <KDebug>
#include <KLocale>
#include <KFileDialog>

#include "config.h"
#include "postscriptdocument.h"
#include "fileprinter.h"

#include "utils/papersizeutils.h"

#include "widgets/printscalingoptionswidget.h"
#include "widgets/posterwidget.h"

/* Return codes:
 * 1: No parameters given. Exit.
 * 2: No PostScript file(s) given. Exit.
 * 3: All PostScript file(s) invalid. Exit.
 * 4: Page rendering error. Exit.
 * 5: Poster print error. Exit.
 * 6: Writing temporary file for data from STDIN failed. Exit.
 */

int showPrintDialogAndPrint(const QString& filename,
                            const QString& printername,
                            const QString& printtitle,
                            const int numCopies,
                            const QStringList& printerOptions,
                            bool nodialog,
                            const QString& system) {

  PostScriptDocument doc;
  if (!doc.load(filename)) {
    kDebug() << "Loading of document " << filename << " failed.";
    return -1;
  }
  int numPages = doc.numPages();
  QPrinter::PaperSize paperSize = doc.paperSize();

  QPrinter printer;
  printer.setPaperSize(paperSize);
  if (!printername.isEmpty()) printer.setPrinterName(printername);
  if (!printtitle.isEmpty()) printer.setDocName(printtitle);
  printer.setCopyCount(numCopies);

  printScalingOptionsWidget scaleWidget;
  PosterWidget posterWidget;

  posterWidget.setMediaSizeDescription(PaperSizeUtils::paperSizeToFancyString(printer.paperSize()));
  posterWidget.setMediaSize(doc.pageSize());

  QObject::connect(&scaleWidget, SIGNAL(scalingEnabled(bool)), &posterWidget, SLOT(setDisabled(bool)));
  QObject::connect(&posterWidget, SIGNAL(posterEnabled(bool)), &scaleWidget, SLOT(setDisabled(bool)));

  QPrintDialog *printDialog = KdePrint::createPrintDialog(&printer, QList<QWidget*>() << &scaleWidget << &posterWidget);
  printDialog->setWindowTitle(i18n("KPrinter4"));
  if (numPages > 0) {
    printDialog->setMinMax(1, numPages);
    printDialog->setFromTo(1, numPages);
  }

  posterWidget.showMultiPageNote((numPages > 1));

  int ret = 0;
  if ((nodialog) || printDialog->exec()) {

    QString pageRange;
    if ((printer.fromPage() > 0) && (printer.toPage() > 0)) {
      pageRange = QString("%1-%2").arg(printer.fromPage()).arg(printer.toPage());
    }

    if (scaleWidget.scaleMode() != printScalingOptionsWidget::NoScale) {

      // Render (selected) pages

      QPainter painter;
      painter.begin(&printer);

      int firstPage = 0;
      int lastPage = numPages;
      if (printer.fromPage() > 0) firstPage = printer.fromPage();
      if (printer.toPage() > 0) lastPage = printer.toPage();

      QMutex mutex;

      doc.renderPagesGS(180, 180);

      QRect viewport = painter.viewport();
      for (int i = firstPage; i < lastPage; ++i) {

        if (i > 0) printer.newPage();

        mutex.lock();

        QImage *pageImage = doc.fetchRenderedPageGS(i);

        if (pageImage) {

          QSizeF pageSize = pageImage->size();
          QSizeF pagePaperSize = printer.paperSize(QPrinter::Point);

          QSize size = scaleWidget.adjustPainterSize(*pageImage, viewport.size(), pageSize, pagePaperSize);
          QPoint pos = scaleWidget.adjustPainterPosition(size, viewport.size());

          painter.setViewport(pos.x(), pos.y(), size.width(), size.height());
          painter.setWindow(pageImage->rect());
          painter.drawImage(0, 0, *pageImage);

          delete pageImage;

        } else {

          kDebug() << "Rendering page" << i << "failed.";
          return 4;

        }

        mutex.unlock();

      }

      doc.clearRenderedPagesGS();

      painter.end();

    } else {

      QString filenameToPrint = filename;

      KTemporaryFile tf;
      if (posterWidget.isEnabled()) {

        bool keepFirstPageOnly = FALSE;
        if (doc.numPages() > 1) {
          kWarning() << "Poster is not able to process multi page postscript documents. Keeping first page only.";
          keepFirstPageOnly = TRUE;
        }

        QStringList argList;

        KTemporaryFile tf2;
        tf2.setSuffix(".ps");
        if (!tf2.open()) {
          kDebug() << "Poster print failed. Creation of temporary file" << tf2.fileName() << "failed.";
          return 5;
        }

        QString filenameToPoster = filename;
        if (keepFirstPageOnly) {
          QString exe = "psselect";
          argList << "-p1";
          argList << filename << tf2.fileName();
          kDebug() << "Executing" << exe << "with arguments" << argList;
          if (KProcess::execute(exe, argList) != 0) {
            kDebug() << "Poster print failed: Execution of" << exe << "failed.";
            return 5;
          }
          filenameToPoster = tf2.fileName();
        }

        // Poster print

        QMap<QString, QString> settings;
        posterWidget.getOptions(settings);

        QString size = settings["_kde-poster-size"];
        QString cut = settings["_kde-poster-cut"];
        QString select = settings["_kde-poster-select"];
        QString printSize = settings["kde-printsize"];

        argList.clear();

        if (!printSize.isEmpty()) {
          argList << QString("-m%1").arg(printSize);
        }
        if (!size.isEmpty()) {
          argList << QString("-p%1").arg(size);
        }
        if (!cut.isEmpty()) {
          argList << QString("-c%1%").arg(cut);
        }
        if (!select.isEmpty()) {
          argList << QString("-P%1").arg(select);
        }

        tf.setSuffix(".ps");
        if (!tf.open()) {
          kDebug() << "Poster print failed. Creation of temporary file" << tf.fileName() << "failed.";
          return 5;
        }

        argList << filenameToPoster << QString("-o%1").arg(tf.fileName());

        QString exe = "poster";
        kDebug() << "Executing" << exe << "with arguments" << argList;

        if (KProcess::execute(exe, argList) != 0) {
          kDebug() << "Poster print failed: Execution of" << exe << "failed.";
          return 5;
        }

        filenameToPrint = tf.fileName();

        pageRange.clear();
        printer.setPrintRange(QPrinter::AllPages);
        printer.setFromTo(0, 0);

      }

      // Just passthrough to CUPS/LPR/LP

      ret = FilePrinter::printFiles(printer, QStringList(filenameToPrint),
        doc.orientation(),
        FilePrinter::ApplicationDeletesFiles,
        FilePrinter::SystemSelectsPages,
        pageRange,
        printerOptions,
        system);

    }

  }

  return ret;

}

int main(int argc, char *argv[]) {

  KAboutData aboutData("kprinter4", 0, ki18n("KPrinter4"), KPRINTER4_VERSION,
                       ki18n("Simple PostScript document printer"),
                       KAboutData::License_GPL,
                       ki18n("Copyright © 2014 by Marco Nelles (credativ GmbH)"),
                       KLocalizedString(),
                       "http://www.credativ.com/",
                       "marco.nelles@credativ.de");
  aboutData.addAuthor(ki18n("Marco Nelles"), ki18n("Current maintainer, main developer"), "marco.nelles@credativ.de");
  aboutData.addCredit(ki18n("City of Munich"), ki18n("LiMux project"), 0, "http://www.muenchen.de/");
  aboutData.addCredit(ki18n("credativ GmbH"), ki18n("credativ GmbH (Germany)"), 0, "http://www.credativ.com/");

  KCmdLineArgs::init(argc, argv, &aboutData);

  KCmdLineOptions options;
  options.add("c", ki18n("Make an internal copy of the files to print"));
  options.add("P").add("d <printer>", ki18n("Printer/destination to print on"));
  options.add("J").add("t <title>", ki18n("Title/Name for the print job"));
  options.add("#").add("n <number>", ki18n("Number of copies"), "1");
  options.add("o <argument> <option=value>", ki18n("Printer/Job option(s)"));
  options.add("j <mode>", ki18n("Job output mode (gui, console, none)"), "gui");
  options.add("system <printsys>", ki18n("Print system to use (autodetect, lpd, cups)"), "autodetect");
  options.add("stdin", ki18n("Print from STDIN"));
  options.add("nd").add("nodialog", ki18n("Do not show the print dialog (print directly)"));
  options.add("ofd").add("openfiledialog", ki18n("Show file dialog instead of printing from STDIN."));
  options.add("+[file(s)]", ki18n("PostScript document(s) to print"));
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app;

  /* Parsing command line arguments */

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if (args->isSet("c")) kWarning() << i18n("Option -c not implemented yet");

  QString printer = args->getOption("d");

  QString title = args->getOption("t");

  int numCopies = 1;
  if (!args->getOption("n").isEmpty()) {
    bool ok;
    int i = args->getOption("n").toInt(&ok);
    if (ok) numCopies = i;
  }

  QStringList printerOptions = args->getOptionList("o");

  QString output_mode = args->getOption("j");
  if ((output_mode != "gui") && (output_mode != "console") && (output_mode != "none")) {
    kWarning() << i18n("Unknown value \"%1\" for option -j. Using default value \"gui\".", output_mode);
    output_mode = "gui";
  }

  QString system = args->getOption("system");
  if ((system != "autodetect") && (system != "cups") && (system != "lpd")) {
    kWarning() << i18n("Unknown value \"%1\" for option --system. Using default value \"autodetect\".", system);
    system = "autodetect";
  }

  bool nodialog = ((args->getOption("j") != "gui") || !args->isSet("nd"));

  QString psFileName;
  KTemporaryFile tmpPSFile;
  if ((args->count()) && (!args->isSet("stdin"))) {

    if (args->count() > 1)
      kWarning() << i18n("Found more than one file parameter. Using only the first one.");

    psFileName = args->url(0).path();

  } else {

    if ((args->isSet("ofd")) && (!args->isSet("stdin"))) {

      psFileName = KFileDialog::getOpenFileName(KUrl(QDir::homePath()), "*.ps", NULL, i18n("Open PostScript document"));

    } else {

      //sync read from stdin

      kWarning() << i18n("Wait for STDIN...");

      QTextStream instream(stdin, QIODevice::ReadOnly);
      QString doc = instream.readAll();

      tmpPSFile.setSuffix(".ps");
      if (tmpPSFile.open()) {
        QTextStream outstream(&tmpPSFile);
        outstream << doc;
        psFileName = tmpPSFile.fileName();
      } else {
        kDebug() << "Error writing temporary file" << tmpPSFile.fileName();
        return 6;
      }

    }

  }

  if (psFileName.isEmpty()) return 2;

  return showPrintDialogAndPrint(psFileName,
        printer,
        title,
        numCopies,
        printerOptions,
        nodialog,
        system);

}
