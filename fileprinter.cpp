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

/* This class is derived from fileprinter class from:
 * Copyright (C) 2007, 2010 by John Layt <john@layt.net>
 */

#include "fileprinter.h"

int FilePrinter::printFiles(QPrinter &printer, const QStringList fileList,
                            QPrinter::Orientation documentOrientation, FileDeletePolicy fileDeletePolicy,
                            PageSelectPolicy pageSelectPolicy, const QString &pageRange) {
  FilePrinter fp;
  return fp.doPrintFiles(printer, fileList, fileDeletePolicy, pageSelectPolicy, pageRange, documentOrientation);
}

int FilePrinter::doPrintFiles(QPrinter &printer, QStringList fileList, FileDeletePolicy fileDeletePolicy,
                              PageSelectPolicy pageSelectPolicy, const QString &pageRange,
                              QPrinter::Orientation documentOrientation) {

  if (fileList.size() < 1) return -8;

  for (QStringList::ConstIterator it = fileList.constBegin(); it != fileList.constEnd(); ++it) {
    if (!QFile::exists(*it)) return -7;
  }

  if (printer.printerState() == QPrinter::Aborted || printer.printerState() == QPrinter::Error) return -6;

  QString exe;
  QStringList argList;
  int ret;

  // Print to File if a filename set, assumes only one file
  if (!printer.outputFileName().isEmpty()) {

    if (QFile::exists(printer.outputFileName())) {
      QFile::remove(printer.outputFileName());
    }

    bool doDeleteFile = (fileDeletePolicy == FilePrinter::SystemDeletesFiles);
    if (printer.outputFormat() == QPrinter::PostScriptFormat) {
      if (!pageRange.isEmpty() && psselectAvailable()) {

        exe = "psselect";
        argList << QString("-p%1").arg(pageRange) << fileList[0] << printer.outputFileName();
        kDebug() << "Executing" << exe << "with arguments" << argList;
        ret = KProcess::execute( exe, argList );

      } else {

        if (doDeleteFile) {
          bool res = QFile::rename(fileList[0], printer.outputFileName());
          if (res) {
            doDeleteFile = FALSE;
            ret = 0;
          } else {
            ret = -5;
          }
        } else {
          bool res = QFile::copy(fileList[0], printer.outputFileName());
          if (res)
            ret = 0;
          else
            ret = -5;
        }

      }
    } else if ((printer.outputFormat() == QPrinter::PdfFormat) && ps2pdfAvailable()) {

      QString inputfilename = fileList[0];
      KTemporaryFile tf;
      if (!pageRange.isEmpty() && psselectAvailable()) {
        tf.setSuffix(".ps");
        if (!tf.open()) return -10;
        exe = "psselect";
        argList << QString("-p%1").arg(pageRange) << inputfilename << tf.fileName();
        kDebug() << "Executing" << exe << "with arguments" << argList;
        ret = KProcess::execute(exe, argList);
        if (ret) return ret;
        inputfilename = tf.fileName();
      }

      exe = "ps2pdf"; argList.clear();
      argList << inputfilename << printer.outputFileName();
      kDebug() << "Executing" << exe << "with arguments" << argList;
      ret = KProcess::execute(exe, argList);

    } else {

      ret = -5;

    }

    if (doDeleteFile) QFile::remove(fileList[0]);

  } else {  // Print to a printer via lpr command

    // Decide what executable to use to print with, need the CUPS version of lpr if available
    // Some distros name the CUPS version of lpr as lpr-cups or lpr.cups so try those first
    // before default to lpr, or failing that to lp

    if (!KStandardDirs::findExe("lpr-cups").isEmpty()) {
      exe = "lpr-cups";
    } else if (!KStandardDirs::findExe("lpr.cups").isEmpty()) {
      exe = "lpr.cups";
    } else if ( !KStandardDirs::findExe("lpr").isEmpty()) {
      exe = "lpr";
    } else if ( !KStandardDirs::findExe("lp").isEmpty()) {
      exe = "lp";
    } else {
      return -9;
    }

    bool useCupsOptions = cupsAvailable();
    argList = printArguments(printer, fileDeletePolicy, pageSelectPolicy,
                             useCupsOptions, pageRange, exe, documentOrientation) << fileList;
    kDebug() << "Executing" << exe << "with arguments" << argList;

    ret = KProcess::execute( exe, argList );

  }

  return ret;

}

QList<int> FilePrinter::pageList(QPrinter &printer, int lastPage, const QList<int> &selectedPageList) {
  return pageList( printer, lastPage, 0, selectedPageList );
}

QList<int> FilePrinter::pageList(QPrinter &printer, int lastPage,
                                 int currentPage, const QList<int> &selectedPageList) {

  if (printer.printRange() == QPrinter::Selection) return selectedPageList;

  int startPage, endPage;
  QList<int> list;

  if (printer.printRange() == QPrinter::PageRange) {
    startPage = printer.fromPage();
    endPage = printer.toPage();
  } else if ( printer.printRange() == QPrinter::CurrentPage) {
    startPage = currentPage;
    endPage = currentPage;
  } else { // All pages
    startPage = 1;
    endPage = lastPage;
  }

  for (int i = startPage; i <= endPage; i++) list << i;

  return list;

}

QString FilePrinter::pageRange(QPrinter &printer, int lastPage, const QList<int> &selectedPageList) {

  if (printer.printRange() == QPrinter::Selection) return pageListToPageRange(selectedPageList);

  if (printer.printRange() == QPrinter::PageRange)
    return QString("%1-%2").arg(printer.fromPage()).arg(printer.toPage());

  return QString("1-%2").arg(lastPage);

}

QString FilePrinter::pageListToPageRange(const QList<int> &pageList) {

  QString pageRange;
  int count = pageList.count();
  int i = 0;
  int seqStart = i;
  int seqEnd;

  while (i != count) {

    if ((i+1 == count) || (pageList[i]+1 != pageList[i+1])) {

      seqEnd = i;

      if (!pageRange.isEmpty()) pageRange.append(",");

      if (seqStart == seqEnd) {
        pageRange.append(pageList[i]);
      } else {
        pageRange.append(QString("%1-%2").arg(seqStart).arg(seqEnd));
      }

      seqStart = i + 1;

    }

    ++i;

  }

  return pageRange;

}

bool FilePrinter::ps2pdfAvailable() {
  return (!KStandardDirs::findExe("ps2pdf").isEmpty());
}

bool FilePrinter::psselectAvailable() {
  return (!KStandardDirs::findExe("psselect").isEmpty());
}

bool FilePrinter::cupsAvailable() {

  // Ideally we would have access to the private Qt method
  // QCUPSSupport::cupsAvailable() to do this as it is very complex routine.
  // However, if CUPS is available then QPrinter::numCopies() will always return 1
  // whereas if CUPS is not available it will return the real number of copies.
  // This behaviour is guaranteed never to change, so we can use it as a reliable substitute.

  QPrinter testPrinter;
  testPrinter.setNumCopies(2);
  return (testPrinter.numCopies() == 1);

}

bool FilePrinter::detectCupsService() {

  QTcpSocket qsock;
  qsock.connectToHost("localhost", 631);
  bool rtn = qsock.waitForConnected() && qsock.isValid();
  qsock.abort();

  return rtn;

}

bool FilePrinter::detectCupsConfig() {

  if (QFile::exists("/etc/cups/cupsd.conf")) return TRUE;
  if (QFile::exists("/usr/etc/cups/cupsd.conf")) return TRUE;
  if (QFile::exists("/usr/local/etc/cups/cupsd.conf")) return TRUE;
  if (QFile::exists("/opt/etc/cups/cupsd.conf")) return TRUE;
  if (QFile::exists("/opt/local/etc/cups/cupsd.conf")) return TRUE;

  return FALSE;

}

QStringList FilePrinter::printArguments(QPrinter &printer, FileDeletePolicy fileDeletePolicy,
                                        PageSelectPolicy pageSelectPolicy, bool useCupsOptions,
                                        const QString &pageRange, const QString &version,
                                        QPrinter::Orientation documentOrientation) {

  QStringList argList;

  if (!destination(printer, version).isEmpty()) argList << destination(printer, version);

  if (!copies(printer, version).isEmpty()) argList << copies(printer, version);

  if (!jobname(printer, version).isEmpty()) argList << jobname(printer, version);

  if (!pages(printer, pageSelectPolicy, pageRange, useCupsOptions, version).isEmpty())
    argList << pages(printer, pageSelectPolicy, pageRange, useCupsOptions, version);

  if (useCupsOptions && !cupsOptions(printer, documentOrientation).isEmpty())
    argList << cupsOptions(printer, documentOrientation);

  if (!deleteFile(printer, fileDeletePolicy, version).isEmpty())
    argList << deleteFile( printer, fileDeletePolicy, version );

  if (version == "lp") argList << "--";

  return argList;

}

QStringList FilePrinter::destination(QPrinter &printer, const QString &version) {

  if (version == "lp") return QStringList("-d") << printer.printerName();

  if (version.startsWith("lpr")) return QStringList("-P") << printer.printerName();

  return QStringList();

}

QStringList FilePrinter::copies(QPrinter &printer, const QString &version) {

  int cp = printer.actualNumCopies();

  if (version == "lp") return QStringList("-n") << QString("%1").arg(cp);

  if (version.startsWith("lpr")) return QStringList() << QString("-#%1").arg(cp);

  return QStringList();

}

QStringList FilePrinter::jobname(QPrinter &printer, const QString &version) {

  if (!printer.docName().isEmpty()) {

    if (version == "lp") return QStringList("-t") << printer.docName();

    if (version.startsWith("lpr")) {
      const QString shortenedDocName = QString::fromUtf8(printer.docName().toUtf8().left(255));
      return QStringList("-J") << shortenedDocName;
    }

  }

  return QStringList();

}

QStringList FilePrinter::deleteFile(QPrinter &printer, FileDeletePolicy fileDeletePolicy, const QString &version) {

  Q_UNUSED(printer);

  if ((fileDeletePolicy == FilePrinter::SystemDeletesFiles) && version.startsWith("lpr")) return QStringList("-r");

  return QStringList();

}

QStringList FilePrinter::pages(QPrinter &printer, PageSelectPolicy pageSelectPolicy, const QString &pageRange,
                               bool useCupsOptions, const QString &version) {

  if (pageSelectPolicy == FilePrinter::SystemSelectsPages) {

    if ((printer.printRange() == QPrinter::Selection) && ! pageRange.isEmpty()) {

      if (version == "lp") return QStringList("-P") << pageRange ;

      if (version.startsWith("lpr") && useCupsOptions)
        return QStringList("-o") << QString("page-ranges=%1").arg( pageRange );

    }

    if (printer.printRange() == QPrinter::PageRange) {

      if (version == "lp")
        return QStringList("-P") << QString("%1-%2").arg(printer.fromPage()).arg(printer.toPage());

      if (version.startsWith("lpr") && useCupsOptions)
        return QStringList("-o") << QString("page-ranges=%1-%2").arg(printer.fromPage()).arg(printer.toPage());

    }

  }

  return QStringList(); // all pages

}

QStringList FilePrinter::cupsOptions(QPrinter &printer, QPrinter::Orientation documentOrientation) {

  QStringList optionList;

  if (!optionMedia(printer).isEmpty() ) optionList << optionMedia( printer );

  if (!optionOrientation(printer, documentOrientation ).isEmpty()) optionList << optionOrientation(printer, documentOrientation);

  if (!optionDoubleSidedPrinting(printer).isEmpty()) optionList << optionDoubleSidedPrinting(printer);

  if (!optionPageOrder(printer).isEmpty()) optionList << optionPageOrder(printer);

  if (!optionCollateCopies(printer).isEmpty()) optionList << optionCollateCopies(printer);

  if (!optionPageMargins(printer).isEmpty()) optionList << optionPageMargins(printer);

  optionList << optionCupsProperties(printer);

  return optionList;

}

QStringList FilePrinter::optionMedia(QPrinter &printer) {

  if (!mediaPageSize(printer).isEmpty() && !mediaPaperSource(printer).isEmpty()) {

    return QStringList("-o") << QString("media=%1,%2").arg(mediaPageSize(printer))
                                .arg( mediaPaperSource(printer));

  }


  if (!mediaPageSize(printer).isEmpty())
    return QStringList("-o") << QString("media=%1").arg(mediaPageSize(printer));

  if (!mediaPaperSource(printer).isEmpty())
    return QStringList("-o") << QString("media=%1").arg(mediaPaperSource(printer));

  return QStringList();

}

QString FilePrinter::mediaPageSize(QPrinter &printer) {

  switch (printer.pageSize()) {

    case QPrinter::A0:         return "A0";
    case QPrinter::A1:         return "A1";
    case QPrinter::A2:         return "A2";
    case QPrinter::A3:         return "A3";
    case QPrinter::A4:         return "A4";
    case QPrinter::A5:         return "A5";
    case QPrinter::A6:         return "A6";
    case QPrinter::A7:         return "A7";
    case QPrinter::A8:         return "A8";
    case QPrinter::A9:         return "A9";
    case QPrinter::B0:         return "B0";
    case QPrinter::B1:         return "B1";
    case QPrinter::B10:        return "B10";
    case QPrinter::B2:         return "B2";
    case QPrinter::B3:         return "B3";
    case QPrinter::B4:         return "B4";
    case QPrinter::B5:         return "B5";
    case QPrinter::B6:         return "B6";
    case QPrinter::B7:         return "B7";
    case QPrinter::B8:         return "B8";
    case QPrinter::B9:         return "B9";
    case QPrinter::C5E:        return "C5";     //Correct Translation?
    case QPrinter::Comm10E:    return "Comm10"; //Correct Translation?
    case QPrinter::DLE:        return "DL";     //Correct Translation?
    case QPrinter::Executive:  return "Executive";
    case QPrinter::Folio:      return "Folio";
    case QPrinter::Ledger:     return "Ledger";
    case QPrinter::Legal:      return "Legal";
    case QPrinter::Letter:     return "Letter";
    case QPrinter::Tabloid:    return "Tabloid";
    case QPrinter::Custom:     return QString("Custom.%1x%2mm").arg(printer.heightMM()).arg(printer.widthMM());
    default:                   return QString();

  }

}

QString FilePrinter::mediaPaperSource(QPrinter &printer) {

  switch (printer.paperSource()) {

    case QPrinter::Auto:            return QString();
    case QPrinter::Cassette:        return "Cassette";
    case QPrinter::Envelope:        return "Envelope";
    case QPrinter::EnvelopeManual:  return "EnvelopeManual";
    case QPrinter::FormSource:      return "FormSource";
    case QPrinter::LargeCapacity:   return "LargeCapacity";
    case QPrinter::LargeFormat:     return "LargeFormat";
    case QPrinter::Lower:           return "Lower";
    case QPrinter::MaxPageSource:   return "MaxPageSource";
    case QPrinter::Middle:          return "Middle";
    case QPrinter::Manual:          return "Manual";
    case QPrinter::OnlyOne:         return "OnlyOne";
    case QPrinter::Tractor:         return "Tractor";
    case QPrinter::SmallFormat:     return "SmallFormat";
    default:                        return QString();

  }

}

QStringList FilePrinter::optionOrientation(QPrinter &printer, QPrinter::Orientation documentOrientation) {

  // portrait and landscape options rotate the document according to the document orientation
  // If we want to print a landscape document as one would expect it, we have to pass the
  // portrait option so that the document is not rotated additionally
  if (printer.orientation() == documentOrientation) {
    // the user wants the document printed as is
    return QStringList("-o") << "portrait";
  }

  // the user expects the document being rotated by 90 degrees
  return QStringList("-o") << "landscape";

}

QStringList FilePrinter::optionDoubleSidedPrinting(QPrinter &printer) {

  switch (printer.duplex()) {

    case QPrinter::DuplexNone:       return QStringList("-o") << "sides=one-sided";
    case QPrinter::DuplexAuto:       if (printer.orientation() == QPrinter::Landscape)
                                       return QStringList("-o") << "sides=two-sided-short-edge";
                                     else
                                       return QStringList("-o") << "sides=two-sided-long-edge";
    case QPrinter::DuplexLongSide:   return QStringList("-o") << "sides=two-sided-long-edge";
    case QPrinter::DuplexShortSide:  return QStringList("-o") << "sides=two-sided-short-edge";
    default:                         return QStringList();  // Use printer default

  }

}

QStringList FilePrinter::optionPageOrder(QPrinter &printer) {

  if (printer.pageOrder() == QPrinter::LastPageFirst) return QStringList("-o") << "outputorder=reverse";

  return QStringList("-o") << "outputorder=normal";

}

QStringList FilePrinter::optionCollateCopies(QPrinter &printer) {

  if (printer.collateCopies()) return QStringList("-o") << "Collate=True";

  return QStringList("-o") << "Collate=False";

}

QStringList FilePrinter::optionPageMargins(QPrinter &printer) {

  if (printer.printEngine()->property(QPrintEngine::PPK_PageMargins).isNull()) {

    return QStringList();

  } else {

    qreal l, t, r, b;
    printer.getPageMargins(&l, &t, &r, &b, QPrinter::Point);
    return QStringList("-o") << QString("page-left=%1").arg(l)
                             <<  "-o"  << QString("page-top=%1").arg(t)
                             <<  "-o"  << QString("page-right=%1").arg(r)
                             <<  "-o"  << QString("page-bottom=%1").arg(b) << "-o" << "fit-to-page";

  }

}

QStringList FilePrinter::optionCupsProperties(QPrinter &printer) {

  QStringList dialogOptions = printer.printEngine()->property(QPrintEngine::PrintEnginePropertyKey(0xfe00)).toStringList();
  QStringList cupsOptions;

  for (int i = 0; i < dialogOptions.count(); i += 2) {

    if (dialogOptions[i+1].isEmpty())
      cupsOptions << "-o" << dialogOptions[i];
    else
      cupsOptions << "-o" << dialogOptions[i] + '=' + dialogOptions[i+1];

  }

  return cupsOptions;

}
