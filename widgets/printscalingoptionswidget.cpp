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

#include "printscalingoptionswidget.h"

printScalingOptionsWidget::printScalingOptionsWidget(QWidget *parent) : printScalingOptionsWidgetUI(parent) {

  initPositionFrame();

  p_alignment = 0;
  p_scalemode = printScalingOptionsWidget::NoScale;
  p_enlarge_smaller_images = FALSE;
  p_scale_factor = 100;

  mScaleGroup.addButton(mNoScale, NoScale);
  mScaleGroup.addButton(mScaleToPage, ScaleToPage);
  mScaleGroup.addButton(mScaleTo, ScaleToCustomSize);

}

printScalingOptionsWidget::~printScalingOptionsWidget() {

}

void printScalingOptionsWidget::initPositionFrame() {

        mPositionFrame->setStyleSheet(
            "QFrame {"
            "   background-color: palette(mid);"
            "   border: 1px solid palette(dark);"
            "}"
            "QToolButton {"
            "   border: none;"
            "   background: palette(base);"
            "}"
            "QToolButton:hover {"
            "   background: palette(alternate-base);"
            "   border: 1px solid palette(highlight);"
            "}"
            "QToolButton:checked {"
            "   background-color: palette(highlight);"
            "}"
        );

        QGridLayout* layout = new QGridLayout(mPositionFrame);
        layout->setMargin(0);
        layout->setSpacing(1);
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                QToolButton* button = new QToolButton(mPositionFrame);
                button->setFixedSize(40, 40);
                button->setCheckable(true);
                layout->addWidget(button, row, col);

                Qt::Alignment alignment;
                if (row == 0) {
                    alignment = Qt::AlignTop;
                } else if (row == 1) {
                    alignment = Qt::AlignVCenter;
                } else {
                    alignment = Qt::AlignBottom;
                }
                if (col == 0) {
                    alignment |= Qt::AlignLeft;
                } else if (col == 1) {
                    alignment |= Qt::AlignHCenter;
                } else {
                    alignment |= Qt::AlignRight;
                }

                mPositionGroup.addButton(button, int(alignment));

                if (alignment & Qt::AlignVCenter && alignment & Qt::AlignHCenter)
                {
                    button->setChecked(true);
                }
            }
        }

}

Qt::Alignment printScalingOptionsWidget::alignment() const {
    int id = p_alignment;
    return Qt::Alignment(id);
}

printScalingOptionsWidget::ScaleMode printScalingOptionsWidget::scaleMode() const {
    return printScalingOptionsWidget::ScaleMode(p_scalemode);
}

bool printScalingOptionsWidget::enlargeSmallerImages() const {
    return p_enlarge_smaller_images;
}

double printScalingOptionsWidget::scaleFactor() const {
    return p_scale_factor / 100.;
}

QSize printScalingOptionsWidget::adjustPainterSize(const QImage& img, const QSize & viewportSize, const QSizeF& pageSize, const QSizeF& paperSize) {
    QSize size = img.size();
    printScalingOptionsWidget::ScaleMode scaleMode = this->scaleMode();
    if (scaleMode == printScalingOptionsWidget::ScaleToPage) {
        bool imageBiggerThanPaper =
            size.width() > viewportSize.width()
            || size.height() > viewportSize.height();

        if (imageBiggerThanPaper || enlargeSmallerImages()) {
            size.scale(viewportSize, Qt::KeepAspectRatio);
        }

    } else if (scaleMode == printScalingOptionsWidget::ScaleToCustomSize) {
        qreal scalePixelX = pageSize.width() / paperSize.width();
        qreal scalePixelY = pageSize.height() / paperSize.height();
        size.scale(viewportSize.width() * scalePixelX * scaleFactor(),
                   viewportSize.height() * scalePixelY * scaleFactor(),
                   Qt::KeepAspectRatio);
    }
    return size;
}

QPoint printScalingOptionsWidget::adjustPainterPosition(const QSize& imageSize, const QSize & viewportSize) {
    Qt::Alignment alignment = this->alignment();
    int posX, posY;

    if (alignment & Qt::AlignLeft) {
        posX = 0;
    } else if (alignment & Qt::AlignHCenter) {
        posX = (viewportSize.width() - imageSize.width()) / 2;
    } else {
        posX = viewportSize.width() - imageSize.width();
    }

    if (alignment & Qt::AlignTop) {
        posY = 0;
    } else if (alignment & Qt::AlignVCenter) {
        posY = (viewportSize.height() - imageSize.height()) / 2;
    } else {
        posY = viewportSize.height() - imageSize.height();
    }

    return QPoint(posX, posY);
}
