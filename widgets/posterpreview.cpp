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

#include "posterpreview.h"

PosterPreview::PosterPreview( QWidget *parent )
    : QFrame( parent )
{
	m_postersize = m_mediasize = "A4";
	m_cutmargin = 5;
	init();
}

PosterPreview::PosterPreview( const QString& postersize, const QString& mediasize, QWidget *parent )
    : QFrame( parent )
{
	m_postersize = postersize;
	m_mediasize = mediasize;
	m_cutmargin = 5;
	init();
}

PosterPreview::~PosterPreview()
{
	delete m_process;
}

void PosterPreview::init()
{
	m_process = new KProcess;
    m_process->setOutputChannelMode(KProcess::SeparateChannels);
    connect( m_process, SIGNAL( readyReadStandardError( ) ), SLOT( slotProcessStderr( ) ) );
    connect( m_process, SIGNAL( finished( int, QProcess::ExitStatus ) ), SLOT( slotProcessExited( int, QProcess::ExitStatus ) ) );

	m_cols = m_rows = m_pw = m_ph = m_mw = m_mh = 0;
	m_dirty = false;
	setDirty();
	setMouseTracking( true );
}

QSize PosterPreview::minimumSizeHint() const
{
    return QSize(10 + m_cols * 25, 10 + m_rows * 25);
}

void PosterPreview::parseBuffer()
{
	int rotate;
	float pw, ph, mw, mh;
	float x1, x2, y1, y2;
    QTextStream posterIn(m_buffer.toLocal8Bit());
    // poster always uses "." as decimal seperator
    posterIn.setLocale(QLocale::c());
    posterIn >> m_rows >> m_cols >> rotate >> pw >> ph >> mw >> mh >> x1 >> y1 >> x2 >> y2;
	m_pw = ( int )( rotate ? ph : pw );
	m_ph = ( int )( rotate ? pw : ph );
	m_mw = ( int )( rotate ? mh : mw );
	m_mh = ( int )( rotate ? mw : mh );
	m_posterbb.setCoords( ( int )x1, ( int )y1, ( int )x2, ( int )y2 );
}

void PosterPreview::setDirty()
{
	if ( !m_dirty )
	{
		m_dirty = true;
		QTimer::singleShot( 1, this, SLOT( updatePoster() ) );
	}
}

void PosterPreview::updatePoster()
{
    m_buffer = "";
    m_process->clearProgram();
    *m_process << "poster" << "-F" << "-m" + m_mediasize << "-p" + m_postersize
        << "-c" + QString::number( m_cutmargin ) + "%";

    m_process->start();

    if ( !m_process->waitForStarted() )
	{
		m_rows = m_cols = 0;
		m_dirty = false;
		update();
	}
}

void PosterPreview::paintEvent( QPaintEvent *ev )
{
    QFrame::paintEvent(ev);

    //QRect rect = contentsRect();
    QPainter p( this );

    //p.fillRect( rect, palette().color(QPalette::Window) );

	if ( isEnabled() )
	{
		if ( m_rows <= 0 || m_cols <= 0 || m_pw <= 0 || m_ph <= 0 )
		{
			QString txt = i18n( "Poster preview not available. Either the <b>poster</b> "
				          "executable is not properly installed, or you don't have "
						  "the required version; available at http://printing.kde.org/downloads/." );
            QTextDocument richtext;
            richtext.setDefaultFont(p.font());
            richtext.setHtml(( m_buffer.isEmpty() ? txt : m_buffer.prepend( "<pre>" ).append( "</pre>" ) ));
			richtext.adjustSize();
            int x = ( width()-richtext.idealWidth() )/2, y = ( height()-richtext.size().height() )/2;
            x = qMax( x, 0 );
            y = qMax( y, 0 );
            //richtext.draw( p, x, y, QRect( x, y, richtext.idealWidth(), richtext.size().height() ), colorGroup() );
            QAbstractTextDocumentLayout::PaintContext context;
            context.palette = palette();
            p.translate(x, y);
            richtext.documentLayout()->draw(&p, context);

			m_boundingrect = QRect();
		}
		else
		{
			int totalx = m_cols*m_pw, totaly = m_rows*m_ph;
            float scale = qMin( float( width()-1 )/totalx, float( height()-1 )/totaly );
            p.translate( 0, height()-1 );
            p.scale( scale, -scale );
			int x = ( int )( width()/scale-totalx )/2, y = ( int )( height()/scale-totaly )/2;
            p.translate( x, y );
            m_boundingrect = p.combinedTransform().mapRect( QRect( 0, 0, totalx, totaly ) );

			x = y = 0;
            int px = m_posterbb.x();
            int py = m_posterbb.y();
            int pw = m_posterbb.width();
            int ph = m_posterbb.height();

			for ( int i=0; i<m_rows; i++, y+=m_ph, x=0 )
			{
				for ( int j=0; j<m_cols; j++, x+=m_pw )
				{
                    bool selected = ( m_selectedpages.contains( i*m_cols+j+1 ) );
                    p.fillRect( x+1, y+1, m_pw-2, m_ph-2, ( selected ? palette().highlight().color() : QColor(Qt::white) ) );
                    p.drawRect( x, y, m_pw, m_ph );
					if ( pw > 0 && ph > 0 )
                        p.fillRect( x+m_mw+px, y+m_mh+py, qMin( pw, m_pw-2*m_mw-px ), qMin( ph, m_ph-2*m_mh-py ),
                                ( selected ? palette().highlight().color().dark( 160 ) : QColor(Qt::lightGray) ) );
                    p.setPen( Qt::DotLine );
                    p.drawRect( x+m_mw, y+m_mh, m_pw-2*m_mw, m_ph-2*m_mh );
                    p.setPen( Qt::SolidLine );

					pw -= m_pw-2*m_mw-px;
					px = 0;
				}

				px = m_posterbb.x();
				ph -= m_ph-2*m_mh-py;
				py = 0;
				pw = m_posterbb.width();
			}
		}
	}
}

void PosterPreview::mouseMoveEvent( QMouseEvent *e )
{
	if ( m_boundingrect.isValid() )
	{
		if ( m_boundingrect.contains( e->pos() ) )
            setCursor( QCursor(Qt::PointingHandCursor) );
		else
            setCursor( QCursor(Qt::ArrowCursor) );
	}
}

void PosterPreview::mousePressEvent( QMouseEvent *e )
{
	if ( e->button() == Qt::LeftButton && m_boundingrect.isValid() )
	{
		if ( m_boundingrect.contains( e->pos() ) )
		{
			int c, r;
			c = ( e->pos().x()-m_boundingrect.x() )/( m_boundingrect.width()/m_cols ) + 1;
			r = m_rows - ( e->pos().y()-m_boundingrect.y() )/( m_boundingrect.height()/m_rows );
			int pagenum = ( r-1 )*m_cols+c;

            if ( !m_selectedpages.contains( pagenum ) ||
                    !( e->modifiers() & Qt::ShiftModifier ) )
			{
                if ( !( e->modifiers() & Qt::ShiftModifier ) )
					m_selectedpages.clear();
				m_selectedpages.append( pagenum );
				update();
				emitSelectedPages();
			}
		}
		else if ( m_selectedpages.count() > 0 )
		{
			m_selectedpages.clear();
			update();
			emitSelectedPages();
		}
	}
}

void PosterPreview::slotProcessStderr()
{
    m_buffer.append(QString::fromLocal8Bit(m_process->readAllStandardError()));
}

void PosterPreview::slotProcessExited( int exitCode, QProcess::ExitStatus exitStatus )
{
    if ( exitStatus == QProcess::NormalExit && exitCode == 0 )
        parseBuffer();
	else
		m_rows = m_cols = 0;

	m_dirty = false;
	update();

    updateGeometry();
}

void PosterPreview::setPosterSize( int s )
{
    setPosterSize( PaperSizeUtils::paperSizeToString( static_cast<QPrinter::PaperSize>(s) ) );
}

void PosterPreview::setPosterSize( const QString& s )
{
	if ( m_postersize != s )
	{
		m_selectedpages.clear();
		m_postersize = s;
		setDirty();
		emitSelectedPages();
	}
}

void PosterPreview::setMediaSize( int s )
{
    setMediaSize( PaperSizeUtils::paperSizeToString( static_cast<QPrinter::PaperSize>(s) ) );
}

void PosterPreview::setMediaSize( const QString& s )
{
	if ( m_mediasize != s )
	{
		m_selectedpages.clear();
		m_mediasize = s;
		setDirty();
		emitSelectedPages();
	}
}

void PosterPreview::setCutMargin( int value )
{
	m_cutmargin = value;
	setDirty();
}

void PosterPreview::setSelectedPages( const QString& s )
{
    QStringList l = s.split(",", QString::SkipEmptyParts);
	m_selectedpages.clear();
    Q_FOREACH (const QString& page, l)
	{
        int p = page.indexOf( '-' );
        if ( p == -1 )
        {
            m_selectedpages.append( page.toInt() );
        }
		else
		{
            int p1 = page.left( p ).toInt(), p2 = page.mid( p+1 ).toInt();
            for ( int i=p1; i<=p2; i++ ) {
				m_selectedpages.append( i );
            }
		}
	}
	update();
}

void PosterPreview::emitSelectedPages()
{
	QString s;
    if ( m_selectedpages.count() > 0 )
	{
        Q_FOREACH (int page, m_selectedpages)
            s.append( QString::number( page ) + "," );
		s.truncate( s.length()-1 );
	}
	emit selectionChanged( s );
}

//#include "posterpreview.moc"
