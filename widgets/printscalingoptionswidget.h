/*
Gwenview: an image viewer
Copyright 2007 Aurélien Gâteau <agateau@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/
#ifndef PRINTOPTIONSPAGE_H
#define PRINTOPTIONSPAGE_H

#include <QWidget>
#include <QButtonGroup>
#include <QGridLayout>
#include <QToolButton>

#include <KConfigDialogManager>
#include <KDebug>
#include <KDialog>

#include "ui_printscalingoptionswidgetUI.h"

class printScalingOptionsWidgetUI : public QWidget, public Ui::PrintScalingOptionsWidgetUI {
public:
  explicit printScalingOptionsWidgetUI(QWidget *parent) : QWidget(parent) {
    setupUi(this);
  }
};

class printScalingOptionsWidget : public printScalingOptionsWidgetUI {

  Q_OBJECT

public:
  enum ScaleMode {
    NoScale,
    ScaleToPage,
    ScaleToCustomSize
  };

  printScalingOptionsWidget(QWidget *parent = NULL);
  ~printScalingOptionsWidget();

  Qt::Alignment alignment() const;
  ScaleMode scaleMode() const;
  bool enlargeSmallerImages() const;
  double scaleFactor() const;

  QSize adjustPainterSize(const QImage& img, const QSize& viewportSize, const QSizeF& pageSize, const QSizeF& paperSize);
  QPoint adjustPainterPosition(const QSize& imageSize, const QSize& viewportSize);

private:
  QButtonGroup mScaleGroup;
  QButtonGroup mPositionGroup;
  KConfigDialogManager* mConfigDialogManager;

  void initPositionFrame();

  int p_alignment;
  ScaleMode p_scalemode;
  bool p_enlarge_smaller_images;
  double p_scale_factor;

};

#endif
