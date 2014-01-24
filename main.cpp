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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <kdeprintdialog.h>
#include <kprintpreview.h>
#include <kdebug.h>
#include <KLocale>

#include "config.h"
#include "psdocument.h"
#include "fileprinter.h"

/* Return codes:
 * 1: No parameters given. Exit.
 * 2: No PostScript file(s) given. Exit.
 */

int main (int argc, char *argv[]) {

  KAboutData aboutData("kprinter4", 0, ki18n("KPrinter4"), KPRINTER4_VERSION,
                       ki18n("Simple PostScript document printer"),
                       KAboutData::License_GPL,
                       ki18n("Copyright © 2014 by Marco Nelles (credativ GmbH)"),
                       KLocalizedString(),
                       "http://www.credativ.com/",
                       "marco.nelles@credativ.de");

  KCmdLineArgs::init(argc, argv, &aboutData);

  KCmdLineOptions options;
  options.add("+[Files]", ki18n("PostScript document(s) to print"));
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app;

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  int ret = 1;
  if (args->count()) {

    PSDocument doc;
    int numPages = 0;

    QStringList psfilenames;
    for (int i = 0; i < args->count(); ++i) {
      QString psfilename = args->url(i).path();
      if (doc.load(psfilename)) {
        psfilenames.append(psfilename);
        numPages += doc.numPages();
      }
    }

    if (psfilenames.count() == 0) return 2;

    QPrinter printer;
    QPrintDialog *printDialog = KdePrint::createPrintDialog(&printer, NULL);

    printDialog->setWindowTitle(i18n("KPrinter4"));
    printDialog->setMinMax(1, numPages);
    printDialog->setFromTo(1, numPages);

    if (printDialog->exec()) {

      QString pageRange;
      if ((printer.fromPage() > 0) && (printer.toPage() > 0))
        pageRange = QString("%1-%2").arg(printer.fromPage()).arg(printer.toPage());

      ret = FilePrinter::printFiles(printer, psfilenames,
        QPrinter::Portrait,
        FilePrinter::ApplicationDeletesFiles,
        FilePrinter::SystemSelectsPages,
        pageRange);

    }

  }

  return ret;

}
