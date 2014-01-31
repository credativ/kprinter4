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

  p_filename = fileName;

  p_internal_document = spectre_document_new();
  spectre_document_load(p_internal_document, QFile::encodeName(p_filename));

  const SpectreStatus loadStatus = spectre_document_status(p_internal_document);
  if (loadStatus != SPECTRE_STATUS_SUCCESS) {
    kDebug() << "ERR:" << spectre_status_to_string(loadStatus);
    spectre_document_free(p_internal_document);
    clear();
    return FALSE;
  }

  p_num_pages = spectre_document_get_n_pages(p_internal_document);
  if (p_num_pages > 0) {
    kDebug() << "Page Count: " << p_num_pages;
  } else {
    kWarning() << "Unable to calculate number of pages.";
    p_num_pages = 0;
  }

  int width, height;
  spectre_document_get_page_size(p_internal_document, &width, &height);
  if ((width > 0) && (height > 0)) {
    p_page_size = p_calc_page_size(QSize(width, height));
    kDebug() << "Page Size: " << width << "x" << height << " (" << p_media_page_size(p_page_size) << ")";
  } else {
    kWarning() << "Unable to calculate page size.";
  }

  p_is_valid = TRUE;
  return TRUE;

}

bool PSDocument::close() {

  spectre_document_free(p_internal_document);
  clear();

  return TRUE;

}

void PSDocument::clear() {

  p_filename.clear();
  p_num_pages = 0;
  p_page_size = DEFAULT_PAGE_SIZE;
  p_is_valid = FALSE;
  p_internal_document = NULL;

}

QPrinter::PaperSize PSDocument::p_calc_page_size(const QSize size) {

  QPrinter::PaperSize result;

       if ((size.width() == 2384) && (size.height() == 3370))   result = QPrinter::A0;
  else if ((size.width() == 1684)  && (size.height() == 2384))  result = QPrinter::A1;
  else if ((size.width() == 1191)  && (size.height() == 1684))  result = QPrinter::A2;
  else if ((size.width() == 842)   && (size.height() == 1191))  result = QPrinter::A3;
  else if ((size.width() == 595)   && (size.height() == 842))   result = QPrinter::A4;
  else if ((size.width() == 596)   && (size.height() == 843))   result = QPrinter::A4;
  else if ((size.width() == 420)   && (size.height() == 595))   result = QPrinter::A5;
  else if ((size.width() == 298)   && (size.height() == 420))   result = QPrinter::A6;
  else if ((size.width() == 210)   && (size.height() == 298))   result = QPrinter::A7;
  else if ((size.width() == 147)   && (size.height() == 210))   result = QPrinter::A8;
  else if ((size.width() == 105)   && (size.height() == 147))   result = QPrinter::A9;
  else if ((size.width() == 283)   && (size.height() == 4008))  result = QPrinter::B0;
  else if ((size.width() == 2004)  && (size.height() == 2835))  result = QPrinter::B1;
  else if ((size.width() == 1417)  && (size.height() == 2004))  result = QPrinter::B2;
  else if ((size.width() == 1001)  && (size.height() == 1417))  result = QPrinter::B3;
  else if ((size.width() == 709)   && (size.height() == 1001))  result = QPrinter::B4;
  else if ((size.width() == 499)   && (size.height() == 709))   result = QPrinter::B5;
  else if ((size.width() == 354)   && (size.height() == 499))   result = QPrinter::B6;
  else if ((size.width() == 249)   && (size.height() == 254))   result = QPrinter::B7;
  else if ((size.width() == 176)   && (size.height() == 249))   result = QPrinter::B8;
  else if ((size.width() == 125)   && (size.height() == 176))   result = QPrinter::B9;
  else if ((size.width() == 88)    && (size.height() == 125))   result = QPrinter::B10;
  else if ((size.width() == 459)   && (size.height() == 649))   result = QPrinter::C5E;
  else if ((size.width() == 297)   && (size.height() == 684))   result = QPrinter::Comm10E;
  else if ((size.width() == 312)   && (size.height() == 624))   result = QPrinter::DLE;
  else if ((size.width() == 522)   && (size.height() == 756))   result = QPrinter::Executive;
  else if ((size.width() == 595)   && (size.height() == 935))   result = QPrinter::Folio;
  else if ((size.width() == 1224)  && (size.height() == 792))   result = QPrinter::Ledger;
  else if ((size.width() == 612)   && (size.height() == 1008))  result = QPrinter::Legal;
  else if ((size.width() == 612)   && (size.height() == 792))   result = QPrinter::Letter;
  else if ((size.width() == 792)   && (size.height() == 1224))  result = QPrinter::Tabloid;
  else result = QPrinter::Custom;

  return result;

}

QString PSDocument::p_media_page_size(const QPrinter::PaperSize size) {

    switch (size) {
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
      case QPrinter::C5E:        return "C5";
      case QPrinter::Comm10E:    return "Comm10";
      case QPrinter::DLE:        return "DL";
      case QPrinter::Executive:  return "Executive";
      case QPrinter::Folio:      return "Folio";
      case QPrinter::Ledger:     return "Ledger";
      case QPrinter::Legal:      return "Legal";
      case QPrinter::Letter:     return "Letter";
      case QPrinter::Tabloid:    return "Tabloid";
      case QPrinter::Custom:     return QString("Custom");
      default:                   return QString();
    }

}
