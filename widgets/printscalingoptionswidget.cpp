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

/*
Gwenview:
Copyright 2007 Aurélien Gâteau <agateau@kde.org>
*/

#include "printscalingoptionswidget.h"

printScalingOptionsWidget::printScalingOptionsWidget(QWidget *parent) : printScalingOptionsWidgetUI(parent) {

  initPositionFrame();

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
            button->setCheckable(TRUE);
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
                    button->setChecked(TRUE);
                }
            }
        }

}

Qt::Alignment printScalingOptionsWidget::alignment() const {
    int id = mPositionGroup.checkedId();
    return Qt::Alignment(id);
}

printScalingOptionsWidget::ScaleMode printScalingOptionsWidget::scaleMode() const {
    return printScalingOptionsWidget::ScaleMode(mScaleGroup.checkedId());
}

bool printScalingOptionsWidget::enlargeSmallerImages() const {
    return kcfg_PrintEnlargeSmallerImages->isChecked();
}

double printScalingOptionsWidget::scaleFactor() const {
    return kcfg_PrintScalePercent->value() / 100.0f;
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
