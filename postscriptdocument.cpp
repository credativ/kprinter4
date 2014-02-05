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

#include "postscriptdocument.h"

PostScriptDocumentPage::PostScriptDocumentPage() {

  clear();

}

PostScriptDocumentPage::PostScriptDocumentPage(const QSize& size, const QPrinter::Orientation orientation, const bool reversePage) {

  p_size = size;
  p_orientation = orientation;
  p_reverse_page = reversePage;
  p_is_valid = TRUE;

}

PostScriptDocumentPage::PostScriptDocumentPage(const PostScriptDocumentPage& other) {

  p_size = other.p_size;
  p_orientation = other.p_orientation;
  p_reverse_page = other.p_reverse_page;
  p_is_valid = other.p_is_valid;

}

PostScriptDocumentPage& PostScriptDocumentPage::operator=(const PostScriptDocumentPage& other) {

  p_size = other.p_size;
  p_orientation = other.p_orientation;
  p_reverse_page = other.p_reverse_page;
  p_is_valid = other.p_is_valid;

  return *this;

}

PostScriptDocumentPage::~PostScriptDocumentPage() {

}

void PostScriptDocumentPage::clear() {

  p_size = QSize(0, 0);
  p_orientation = DEFAULT_ORIENTATION;
  p_reverse_page = FALSE;
  p_is_valid = FALSE;

}



PostScriptDocument::PostScriptDocument() {

}

PostScriptDocument::PostScriptDocument(const QString& fileName) {

  load(fileName);

}

PostScriptDocument::~PostScriptDocument() {

}

bool PostScriptDocument::load(const QString& fileName) {

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

  int numPages = spectre_document_get_n_pages(p_internal_document);
  if (numPages > 0) {
    kDebug() << "Page Count: " << numPages;
  } else {
    kWarning() << "Unable to calculate number of pages.";
    numPages = 0;
  }

  int width, height;
  spectre_document_get_page_size(p_internal_document, &width, &height);
  if ((width > 0) && (height > 0)) {
    p_page_size = QSize(width, height);
    kDebug() << "Page Size: " << width << "x" << height << " (" << PaperSizeUtils::paperSizeToString(PaperSizeUtils::sizeToPaperSize(p_page_size)) << ")";
  } else {
    kWarning() << "Unable to calculate page size.";
  }

  SpectreOrientation orientation = spectre_document_get_orientation(p_internal_document);
  bool reversePage;
  p_orientation = spectreOrientationToOrientation(orientation, &reversePage);
  kDebug() << "Page Orientation: " << PaperSizeUtils::orientationToString(p_orientation);
  kDebug() << "Note: The page orientation may differ from orientation for each page.";

  /* Load the pages now */
  SpectrePage *page;
  SpectreOrientation pageOrientation;
  QPrinter::Orientation pageOrientation2;
  width = 0; height = 0;
  for (int i = 0; i < numPages; ++i) {

    pageOrientation = SPECTRE_ORIENTATION_PORTRAIT;
    page = spectre_document_get_page(p_internal_document, i);
    if (spectre_document_status(p_internal_document)) {
      kWarning() << "Error getting page " << i << spectre_status_to_string(spectre_document_status(p_internal_document));
    } else {
      spectre_page_get_size(page, &width, &height);
      pageOrientation = spectre_page_get_orientation(page);
    }
    spectre_page_free(page);

    pageOrientation2 = spectreOrientationToOrientation(pageOrientation, &reversePage);

    p_pages.append(PostScriptDocumentPage(QSize(width, height), pageOrientation2, reversePage));
    kDebug() << "Append page" << i+1 << "with Size (" << width << "," << height << ")," << PaperSizeUtils::orientationToString(pageOrientation2);

  }
  kDebug() << "Loaded" << p_pages.count() << "pages";

  p_is_valid = TRUE;
  return TRUE;

}

bool PostScriptDocument::close() {

  spectre_document_free(p_internal_document);
  clear();

  return TRUE;

}

void PostScriptDocument::clear() {

  p_filename.clear();
  p_pages.clear();
  p_page_size = QSize();
  p_orientation = DEFAULT_ORIENTATION;
  p_is_valid = FALSE;
  p_internal_document = NULL;

}

QImage* PostScriptDocument::renderPage(const int pageNum, const int dpiX, const int dpiY) {

  Q_UNUSED(dpiX);
  Q_UNUSED(dpiY);

  if ((pageNum < 0) || (pageNum >= p_pages.count())) return NULL;

  PostScriptDocumentPage page = p_pages[pageNum];
  /*int width = reqSize.width();
  int height = reqSize.height();
  double magnify = 1.0f;
  if (page.orientation() == QPrinter::Landscape) {
    magnify = qMax((double)height / (double)page.size().width(),
                   (double)width / (double)page.size().height());
  } else {
    magnify = qMax((double)width / (double)page.size().width(),
                   (double)height / (double)page.size().height());
  }*/

  SpectrePage *spage = spectre_document_get_page(p_internal_document, pageNum);

  SpectreRenderContext *renderContext = spectre_render_context_new();

  /*spectre_render_context_set_scale(renderContext, magnify, magnify);*/
  spectre_render_context_set_use_platform_fonts(renderContext, false);
  spectre_render_context_set_antialias_bits(renderContext, 4, 4);
  /* Do not use spectre_render_context_set_rotation makes some files not render correctly, e.g. bug210499.ps
   * so we basically do the rendering without any rotation and then rotate to the orientation as needed
   * spectre_render_context_set_rotation(m_renderContext, req.orientation);
   */

  unsigned char *data = NULL;
  int row_length = 0;

  spectre_page_render(spage, renderContext, &data, &row_length);

  if (spectre_page_status(spage) != SPECTRE_STATUS_SUCCESS) {
    kDebug() << "Failed to render page" << pageNum+1 << ". Spectre fail status:" << spectre_page_status(spage);
    return NULL;
  }

  int width = page.size().width();
  int height = page.size().height();

  kDebug() << "Size of page" << pageNum+1 << "to render: " << width << "x" << height;

  // Qt4 needs the missing alpha of QImage::Format_RGB32 to be 0xff
  if (data && data[3] != 0xff)
    for (int i = 3; i < row_length * height; i += 4)
      data[i] = 0xff;

  QImage image;

  if (row_length == width * 4) {
    image = QImage(data, width, height, QImage::Format_RGB32);
  } else {
    // In case this ends up beign very slow we can try with some memmove
    QImage aux(data, row_length / 4, height, QImage::Format_RGB32);
    image = QImage(aux.copy(0, 0, width, height));
  }

  /*if (page.reversePage()) {

    if (page.orientation() == QPrinter::Portrait) {
      QTransform m;
      m.rotate(180);
      image = image.transformed(m);
    } else if (page.orientation() == QPrinter::Landscape) {
      QTransform m;
      m.rotate(270);
      image = image.transformed(m);
    }

  } else {

    if (page.orientation() == QPrinter::Landscape) {
      QTransform m;
      m.rotate(90);
      image = image.transformed(m);
    }

  }*/

  QImage *result = new QImage(image.copy());

  if ((result->width() != width) || (result->height() != height)) {
    kWarning().nospace() << "Generated image does not match wanted size: "
                    << "[" << result->width() << "x" << result->height() << "] vs requested "
                    << "[" << width << "x" << height << "]";
    QImage aux = result->scaled(width, height);
    delete result;
    result = new QImage(aux);
  }

  spectre_page_free(spage);

  spectre_render_context_free(renderContext);

  return result;

}

QImage* PostScriptDocument::renderPageGS(const int pageNum, const int dpiX, const int dpiY) {

  Q_UNUSED(dpiX);
  Q_UNUSED(dpiY);

  if ((pageNum < 0) || (pageNum >= p_pages.count())) return NULL;

  PostScriptDocumentPage page = p_pages[pageNum];
  QString paperSize = PaperSizeUtils::paperSizeToString(PaperSizeUtils::sizeToPaperSize(page.size()));

  KTemporaryFile tf;
  tf.setSuffix(".png");
  tf.setAutoRemove(FALSE);

  if (!tf.open()) {
    kDebug() << "Rendering page" << pageNum+1 << "failed: Creation of temporary file" << tf.fileName() << "failed.";
    return NULL;
  }

  QStringList args;

  args << "-q";
  args << "-dNOPAUSE";
  args << "-dSAFER";
  args << "-dQUIET";
  args << "-dBATCH";
  args << "-dNOPROMPT";
  //args << QString("-r=%1x%2").arg(dpiX).arg(dpiY);
  args << QString("-r180");
  args << QString("-sPAPERSIZE=%1").arg(paperSize.toLower());
  args << QString("-dFirstPage=%1").arg(pageNum+1);
  args << QString("-dLastPage=%1").arg(pageNum+1);
  args << "-sDEVICE=png16m";
  args << "-dTextAlphaBits=4";
  args << "-dGraphicsAlphaBits=4";
  args << QString("-sOutputFile=%1").arg(tf.fileName());

  args << p_filename;

  kDebug() << "Executing" << "gs" << "with arguments" << args;

  if (KProcess::execute("gs", args) != 0) {
    kDebug() << "Rendering page" << pageNum+1 << "failed: Execution of GhostScript (gs) failed.";
    return NULL;
  }

  int width = page.size().width();
  int height = page.size().height();

  kDebug() << "Size of page" << pageNum+1 << "to render: " << width << "x" << height;

  QImage image(tf.fileName());

  QImage *result = new QImage(image.copy());

  if ((result->width() != width) || (result->height() != height)) {
    kWarning().nospace() << "Generated image does not match wanted size: "
                    << "[" << result->width() << "x" << result->height() << "] vs requested "
                    << "[" << width << "x" << height << "]";
    QImage aux = result->scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    delete result;
    result = new QImage(aux);
  }

  return result;

}

QPrinter::Orientation PostScriptDocument::spectreOrientationToOrientation(SpectreOrientation orientation, bool *reversePage) {

  *reversePage = TRUE;

  switch (orientation) {
    case SPECTRE_ORIENTATION_PORTRAIT : *reversePage = FALSE;
    case SPECTRE_ORIENTATION_REVERSE_PORTRAIT : return QPrinter::Portrait;
    case SPECTRE_ORIENTATION_LANDSCAPE : *reversePage = FALSE;
    case SPECTRE_ORIENTATION_REVERSE_LANDSCAPE : return QPrinter::Landscape;
  }

  return DEFAULT_ORIENTATION;

}
