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

#include "psdocument.h"

PSDocument::PSDocument() {

}

PSDocument::PSDocument(const QString& fileName) {

  load(fileName);

}

PSDocument::~PSDocument() {

}

bool PSDocument::load(const QString& fileName) {

  clear();

  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    return FALSE;
  }

  QByteArray line = file.readLine();
  if (!line.startsWith("%!PS-Adobe-")) {
    return FALSE;
  }

  while (!file.atEnd()) {

    if (line.indexOf("%%Pages:") > -1) {
      int PagesNumPos = line.indexOf("%%Pages:");
      p_num_pages = line.mid(PagesNumPos+8).trimmed().toInt();
    }

    if (line.indexOf("<</PageSize") > -1) {
      int b1 = line.indexOf('[')+1;
      int b2 = line.indexOf(']')-1;
      if ((b1 == -1) || (b2 == -1) || (b2 < b1)) continue;
      p_paper_size = p_calc_paper_size(line.mid(b1, b2-b1).trimmed());
    }

    line = file.readLine();

  }

  file.close();

  p_is_valid = TRUE;
  return TRUE;

}

void PSDocument::clear() {

  p_filename.clear();
  p_num_pages = -1;
  p_paper_size = DEFAULT_PAPER_SIZE;
  p_is_valid = FALSE;

}

QPrinter::PaperSize PSDocument::p_calc_paper_size(const QString size) {

  QPrinter::PaperSize result;

       if (size == "2384 3370") result = QPrinter::A0;
  else if (size == "1684 2384") result = QPrinter::A1;
  else if (size == "1191 1684") result = QPrinter::A2;
  else if (size == "842 1191")  result = QPrinter::A3;
  else if (size == "595 842")   result = QPrinter::A4;
  else if (size == "420 595")   result = QPrinter::A5;
  else if (size == "298 420")   result = QPrinter::A6;
  else if (size == "210 298")   result = QPrinter::A7;
  else if (size == "147 210")   result = QPrinter::A8;
  else if (size == "105 147")   result = QPrinter::A9;
  else if (size == "283 4008")  result = QPrinter::B0;
  else if (size == "2004 2835") result = QPrinter::B1;
  else if (size == "1417 2004") result = QPrinter::B2;
  else if (size == "1001 1417") result = QPrinter::B3;
  else if (size == "709 1001")  result = QPrinter::B4;
  else if (size == "499 709")   result = QPrinter::B5;
  else if (size == "354 499")   result = QPrinter::B6;
  else if (size == "249 254")   result = QPrinter::B7;
  else if (size == "176 249")   result = QPrinter::B8;
  else if (size == "125 176")   result = QPrinter::B9;
  else if (size == "88 125")    result = QPrinter::B10;
  else if (size == "459 649")   result = QPrinter::C5E;
  else if (size == "297 684")   result = QPrinter::Comm10E;
  else if (size == "312 624")   result = QPrinter::DLE;
  else if (size == "522 756")   result = QPrinter::Executive;
  else if (size == "595 935")   result = QPrinter::Folio;
  else if (size == "1224 792")  result = QPrinter::Ledger;
  else if (size == "612 1008")  result = QPrinter::Legal;
  else if (size == "612 792")   result = QPrinter::Letter;
  else if (size == "792 1224")  result = QPrinter::Tabloid;
  else result = QPrinter::Custom;

  return result;

}
