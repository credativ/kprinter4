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

/* Copyright (c) 2013 Felix Geyer <felix.geyer@credativ.de> */

#include "papersizeutils.h"

QList<QPrinter::PaperSize> PaperSizeUtils::allPaperSizes() {

  QList<QPrinter::PaperSize> result;

  result.append(QPrinter::A4);
  result.append(QPrinter::B5);
  result.append(QPrinter::Letter);
  result.append(QPrinter::Legal);
  result.append(QPrinter::Executive);
  result.append(QPrinter::A0);
  result.append(QPrinter::A1);
  result.append(QPrinter::A2);
  result.append(QPrinter::A3);
  result.append(QPrinter::A5);
  result.append(QPrinter::A6);
  result.append(QPrinter::A7);
  result.append(QPrinter::A8);
  result.append(QPrinter::A9);
  result.append(QPrinter::B0);
  result.append(QPrinter::B1);
  result.append(QPrinter::B10);
  result.append(QPrinter::B2);
  result.append(QPrinter::B3);
  result.append(QPrinter::B4);
  result.append(QPrinter::B6);
  result.append(QPrinter::B7);
  result.append(QPrinter::B8);
  result.append(QPrinter::B9);
  result.append(QPrinter::C5E);
  result.append(QPrinter::Comm10E);
  result.append(QPrinter::DLE);
  result.append(QPrinter::Folio);
  result.append(QPrinter::Ledger);
  result.append(QPrinter::Tabloid);

  return result;

}

QPrinter::PaperSize PaperSizeUtils::sizeToPaperSize(const QSize size, const bool transpose) {

  QPrinter::PaperSize result;

  QSize tmp = size;
  if (transpose) tmp.transpose();

       if ((tmp.width() == 2384)  && (tmp.height() == 3370))  result = QPrinter::A0;
  else if ((tmp.width() == 1684)  && (tmp.height() == 2384))  result = QPrinter::A1;
  else if ((tmp.width() == 1191)  && (tmp.height() == 1684))  result = QPrinter::A2;
  else if ((tmp.width() == 842)   && (tmp.height() == 1191))  result = QPrinter::A3;
  else if ((tmp.width() == 595)   && (tmp.height() == 842))   result = QPrinter::A4;
  else if ((tmp.width() == 596)   && (tmp.height() == 843))   result = QPrinter::A4;
  else if ((tmp.width() == 420)   && (tmp.height() == 595))   result = QPrinter::A5;
  else if ((tmp.width() == 298)   && (tmp.height() == 420))   result = QPrinter::A6;
  else if ((tmp.width() == 210)   && (tmp.height() == 298))   result = QPrinter::A7;
  else if ((tmp.width() == 147)   && (tmp.height() == 210))   result = QPrinter::A8;
  else if ((tmp.width() == 105)   && (tmp.height() == 147))   result = QPrinter::A9;
  else if ((tmp.width() == 283)   && (tmp.height() == 4008))  result = QPrinter::B0;
  else if ((tmp.width() == 2004)  && (tmp.height() == 2835))  result = QPrinter::B1;
  else if ((tmp.width() == 1417)  && (tmp.height() == 2004))  result = QPrinter::B2;
  else if ((tmp.width() == 1001)  && (tmp.height() == 1417))  result = QPrinter::B3;
  else if ((tmp.width() == 709)   && (tmp.height() == 1001))  result = QPrinter::B4;
  else if ((tmp.width() == 499)   && (tmp.height() == 709))   result = QPrinter::B5;
  else if ((tmp.width() == 354)   && (tmp.height() == 499))   result = QPrinter::B6;
  else if ((tmp.width() == 249)   && (tmp.height() == 254))   result = QPrinter::B7;
  else if ((tmp.width() == 176)   && (tmp.height() == 249))   result = QPrinter::B8;
  else if ((tmp.width() == 125)   && (tmp.height() == 176))   result = QPrinter::B9;
  else if ((tmp.width() == 88)    && (tmp.height() == 125))   result = QPrinter::B10;
  else if ((tmp.width() == 459)   && (tmp.height() == 649))   result = QPrinter::C5E;
  else if ((tmp.width() == 297)   && (tmp.height() == 684))   result = QPrinter::Comm10E;
  else if ((tmp.width() == 312)   && (tmp.height() == 624))   result = QPrinter::DLE;
  else if ((tmp.width() == 522)   && (tmp.height() == 756))   result = QPrinter::Executive;
  else if ((tmp.width() == 595)   && (tmp.height() == 935))   result = QPrinter::Folio;
  else if ((tmp.width() == 1224)  && (tmp.height() == 792))   result = QPrinter::Ledger;
  else if ((tmp.width() == 612)   && (tmp.height() == 1008))  result = QPrinter::Legal;
  else if ((tmp.width() == 612)   && (tmp.height() == 792))   result = QPrinter::Letter;
  else if ((tmp.width() == 792)   && (tmp.height() == 1224))  result = QPrinter::Tabloid;
  else if (!transpose) result = sizeToPaperSize(tmp, TRUE); else result = QPrinter::Custom;

  return result;

}

QString PaperSizeUtils::paperSizeToString(const QPrinter::PaperSize size) {

  switch (size) {
      case QPrinter::A0 :         return "A0";
      case QPrinter::A1 :         return "A1";
      case QPrinter::A2 :         return "A2";
      case QPrinter::A3 :         return "A3";
      case QPrinter::A4 :         return "A4";
      case QPrinter::A5 :         return "A5";
      case QPrinter::A6 :         return "A6";
      case QPrinter::A7 :         return "A7";
      case QPrinter::A8 :         return "A8";
      case QPrinter::A9 :         return "A9";
      case QPrinter::B0 :         return "B0";
      case QPrinter::B1 :         return "B1";
      case QPrinter::B10 :        return "B10";
      case QPrinter::B2 :         return "B2";
      case QPrinter::B3 :         return "B3";
      case QPrinter::B4 :         return "B4";
      case QPrinter::B5 :         return "B5";
      case QPrinter::B6 :         return "B6";
      case QPrinter::B7 :         return "B7";
      case QPrinter::B8 :         return "B8";
      case QPrinter::B9 :         return "B9";
      case QPrinter::C5E :        return "C5";
      case QPrinter::Comm10E :    return "Comm10";
      case QPrinter::DLE :        return "DL";
      case QPrinter::Executive :  return "Executive";
      case QPrinter::Folio :      return "Folio";
      case QPrinter::Ledger :     return "Ledger";
      case QPrinter::Legal :      return "Legal";
      case QPrinter::Letter :     return "Letter";
      case QPrinter::Tabloid :    return "Tabloid";
      case QPrinter::Custom :     return QString("Custom");
      default :                   return QString();
    }

}

QString PaperSizeUtils::paperSizeToFancyString(const QPrinter::PaperSize size) {

  switch (size) {
      case QPrinter::A0 :         return "DIN A0";
      case QPrinter::A1 :         return "DIN A1";
      case QPrinter::A2 :         return "DIN A2";
      case QPrinter::A3 :         return "DIN A3";
      case QPrinter::A4 :         return "DIN A4";
      case QPrinter::A5 :         return "DIN A5";
      case QPrinter::A6 :         return "DIN A6";
      case QPrinter::A7 :         return "DIN A7";
      case QPrinter::A8 :         return "DIN A8";
      case QPrinter::A9 :         return "DIN A9";
      case QPrinter::B0 :         return "B0";
      case QPrinter::B1 :         return "B1";
      case QPrinter::B10 :        return "B10";
      case QPrinter::B2 :         return "B2";
      case QPrinter::B3 :         return "B3";
      case QPrinter::B4 :         return "B4";
      case QPrinter::B5 :         return "B5";
      case QPrinter::B6 :         return "B6";
      case QPrinter::B7 :         return "B7";
      case QPrinter::B8 :         return "B8";
      case QPrinter::B9 :         return "B9";
      case QPrinter::C5E :        return "C5";
      case QPrinter::Comm10E :    return "Commercial#10";
      case QPrinter::DLE :        return "DL";
      case QPrinter::Executive :  return "Executive";
      case QPrinter::Folio :      return "Folio";
      case QPrinter::Ledger :     return "Ledger";
      case QPrinter::Legal :      return "Legal";
      case QPrinter::Letter :     return "Letter";
      case QPrinter::Tabloid :    return "Tabloid";
      case QPrinter::Custom :     return QString("Custom");
      default :                   return QString();
    }

}

QString PaperSizeUtils::orientationToString(const QPrinter::Orientation orientation) {

  switch (orientation) {
    case QPrinter::Portrait :   return i18n("Portrait");
    case QPrinter::Landscape :  return i18n("Landscape");
  }

  return QString();

}
