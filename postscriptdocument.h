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
#include <QImage>

#include <KDebug>
#include <KLocale>
#include <KProcess>
#include <KTemporaryFile>

#include <libspectre/spectre.h>

#include <utils/papersizeutils.h>

#define DEFAULT_PAPER_SIZE QPrinter::A4
#define DEFAULT_ORIENTATION QPrinter::Portrait

class PostScriptDocument; // forward declaration for static members

class PostScriptDocumentPage {

public:
  PostScriptDocumentPage();
  PostScriptDocumentPage(const QSize& size, const QPrinter::Orientation orientation, const bool reversePage);
  PostScriptDocumentPage(const PostScriptDocumentPage& other);
  PostScriptDocumentPage& operator=(const PostScriptDocumentPage& other);
  ~PostScriptDocumentPage();

  inline QSize size() { return p_size; }
  inline QPrinter::Orientation orientation() { return p_orientation; }
  inline bool reversePage() { return p_reverse_page; }

  void clear();

  inline bool isValid() { return p_is_valid; }

private:
  QSize p_size;
  QPrinter::Orientation p_orientation;
  bool p_reverse_page;

  bool p_is_valid;

};

class PostScriptDocument {

public:
  PostScriptDocument();
  PostScriptDocument(const QString& fileName);
  ~PostScriptDocument();

  bool load(const QString& fileName);
  bool close();

  void clear();

  /* Requested size depends on physical parameters of the hardware */
  QImage* renderPage(const int pageNum, const int dpiX, const int dpiY);

  /* Alternative implementation with GhostScript */
  QImage* renderPageGS(const int pageNum, const int dpiX, const int dpiY);

  inline int numPages() { return p_pages.count(); }
  inline QSize pageSize() { return p_page_size; }
  inline QPrinter::PaperSize paperSize() { return PaperSizeUtils::sizeToPaperSize(p_page_size); }
  inline QPrinter::Orientation orientation() { return p_orientation; }

  inline PostScriptDocumentPage& page(const int num) { if ((num >= 0) || (num < p_pages.count())) return p_pages[num]; }

  inline bool isValid() { return p_is_valid; }

  static QPrinter::Orientation spectreOrientationToOrientation(SpectreOrientation orientation, bool *reversePage);

private:
  QString p_filename;

  SpectreDocument *p_internal_document;

  QList<PostScriptDocumentPage> p_pages;

  bool p_is_valid;

  QSize p_page_size;
  QPrinter::Orientation p_orientation;

};

#endif
