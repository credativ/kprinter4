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

#ifndef POSTERPREVIEW_H
#define POSTERPREVIEW_H

#include <stdio.h>

#include <QFrame>
#include <QPainter>
#include <QTimer>
#include <QPixmap>
#include <QTextDocument>
#include <QColorGroup>
#include <QAbstractTextDocumentLayout>
#include <QTextStream>

#include <KProcess>
#include <KLocale>
#include <KCursor>
#include <KGlobalSettings>

#include "utils/papersizeutils.h"

class PosterPreview : public QFrame
{
	Q_OBJECT
public:
    PosterPreview( QWidget *parent = 0 );
    PosterPreview( const QString& postersize, const QString& mediasize, QWidget *parent = 0 );
	~PosterPreview();
    QSize minimumSizeHint() const;

public slots:
	void setPosterSize( int );
	void setPosterSize( const QString& );
	void setMediaSize( int );
	void setMediaSize( const QString& );
	void setCutMargin( int );
	void updatePoster();
	void setSelectedPages( const QString& );

signals:
	void selectionChanged( const QString& );

protected:
	void parseBuffer();
    void paintEvent( QPaintEvent* );
	void init();
	void setDirty();
	void mouseMoveEvent( QMouseEvent* );
	void mousePressEvent( QMouseEvent* );
	void emitSelectedPages();

protected slots:
    void slotProcessStderr();
    void slotProcessExited( int exitCode, QProcess::ExitStatus exitStatus );

private:
	int m_rows, m_cols;
	int m_pw, m_ph; // page size
	int m_mw, m_mh; // cur margins
	QRect m_posterbb; // poster bounding box (without any margin)
	KProcess *m_process;
	QString m_buffer;
	QString m_postersize, m_mediasize;
	int m_cutmargin;
	bool m_dirty;
	QRect m_boundingrect;
    QList<int> m_selectedpages;
};

#endif /* POSTERPREVIEW_H */
