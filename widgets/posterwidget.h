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

/* Copyright (c) 2001-2002 Michael Goffioul <kdeprint@swing.be> */

#ifndef POSTERWIDGET_H
#define POSTERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <KIntNumInput>

#include "utils/papersizeutils.h"
#include "posterpreview.h"

class PosterWidget : public QWidget
{
	Q_OBJECT
    Q_PROPERTY(QSizeF mediaSize READ mediaSize WRITE setMediaSize)
    Q_PROPERTY(QString mediaSizeDescription READ mediaSizeDescription WRITE setMediaSizeDescription)

public:
    PosterWidget( QWidget *parent = 0 );
    ~PosterWidget();

    void getOptions( QMap<QString,QString>& opts, bool incldef = false );
    bool isEnabled();

    QSizeF mediaSize() const;
    void setMediaSize(const QSizeF& mediaSizeFloat);

    QString mediaSizeDescription() const;
    void setMediaSizeDescription(const QString& mediaSize);

    void showMultiPageNote(const bool show);

signals:
    void posterEnabled(bool enabled);

protected slots:
	void slotPosterSizeChanged( int );
    void slotPrintSizeChanged( int );
	void slotMarginChanged( int );
	void slotLockToggled( bool );

private:
	QComboBox *m_postersize;
	QComboBox *m_printsize;
	PosterPreview *m_preview;
	QCheckBox *m_postercheck;
	QLabel *m_mediasize;
	KIntNumInput *m_cutmargin;
	QPushButton *m_lockbtn;
	QLineEdit *m_selection;
        QLabel *m_multipage_note_label;
};

#endif
