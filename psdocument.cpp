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

  bool ok; int PagesNumPos;
  while (!file.atEnd()) {
    PagesNumPos = line.indexOf("%%Pages:");
    if (PagesNumPos > -1) {
      p_num_pages = line.mid(PagesNumPos+8).trimmed().toInt(&ok);
      if (ok) break;
    }
    line = file.readLine();
  }

  file.close();

  p_is_valid = TRUE;
  return TRUE;

}

void PSDocument::clear() {

  p_filename.clear();
  p_num_pages = 0;
  p_is_valid = FALSE;

}
