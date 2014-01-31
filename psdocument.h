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

/* This class handles a PostScript-Document with libspectre */

#ifndef PSDOCUMENT_HEADER
#define PSDOCUMENT_HEADER

#include <QString>
#include <QStringList>
#include <QBuffer>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QSize>
#include <QPrinter>

#include <KDebug>
#include <KLocale>

#include <libspectre/spectre.h>

#define DEFAULT_PAGE_SIZE QPrinter::A4

class PSDocument {

public:
  PSDocument();
  PSDocument(const QString& fileName);
  ~PSDocument();

  bool load(const QString& fileName);
  bool close();

  void clear();

  inline int numPages() { return p_num_pages; }
  inline QPrinter::PaperSize pageSize() { return p_page_size; }

private:
  QString p_filename;

  SpectreDocument *p_internal_document;

  bool p_is_valid;

  QPrinter::PaperSize p_calc_page_size(const QSize size);
  QString p_media_page_size(const QPrinter::PaperSize size);

  int p_num_pages;
  QPrinter::PaperSize p_page_size;

};

#endif
