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

#ifndef PAPERSIZEUTILS_H
#define PAPERSIZEUTILS_H

#include <QPair>
#include <QPrinter>

#include <KLocale>

class PaperSizeUtils {

public:
  static QList<QPrinter::PaperSize> allPaperSizes();
  static QPrinter::PaperSize sizeToPaperSize(const QSize size, const bool transpose = FALSE);
  static QString paperSizeToString(const QPrinter::PaperSize size);
  static QString paperSizeToFancyString(const QPrinter::PaperSize size);
  static QString orientationToString(const QPrinter::Orientation orientation);

};

#endif
