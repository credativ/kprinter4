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

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QToolTip>
#include <QLineEdit>
#include <QWhatsThis>

#include <KPushButton>
#include <KLocale>
#include <KNumInput>
#include <KIconLoader>

#include "posterwidget.h"
#include "posterpreview.h"

PosterWidget::PosterWidget( QWidget *parent )
    : QWidget( parent )
{
        //WhatsThis strings.... (added by pfeifle@kde.org)
	QString whatsThis5_PosterPage = i18n( " <qt> "
			" 5. "
			" </qt>" );

	QString whatsThisEnablePosterPage = i18n( " <qt> "
                        " <b>Print Poster</b> (enabled or disabled). "
			" <p>If you enable this option, you can print posters of different sizes "
			" The printout will happen in the form <em>'tiles'</em> printed on smaller "
			" paper sizes, which you can stitch together later. <em>If you enable this "
			" option here, the <em>'Poster Printing' filter</em> will be auto-loaded in "
			" the 'Filters' tab of this dialog. </p>"
			" <p>This tab is only visible if the external <em>'poster'</em> utility is "
			" discovered by KDEPrint on your system. [<em>'poster'</em> is a commandline "
			" utility that enables you to convert PostScript files into tiled printouts "
			" which allow for oversized appearance of the stitched-together tiles.] </p>"
			" <p><b>Note:</b> The standard version of 'poster' will not work. Your system "
			" must use a patched version of 'poster'. Ask your operating system vendor to "
			" provide a patched version of 'poster' if he does not already. </p>"
			" <p><b>Additional hint for power users:</b> A patched version of 'poster' is "
			" available from the <a href=\"http://printing.kde.org/\">KDEPrint Website</a> "
			" at <a href=\"http://printing.kde.org/downloads/\">http://printing.kde.org/downloads/</a>. "
			" The direct link to the patched source tarball is "
			" <a href=\"ftp://ftp.kde.org/pub/kde/printing/poster.tar.bz2\">ftp://ftp.kde.org/pub/kde/printing/poster.tar.bz2</a> "
			" </p> "
			" </qt>" );

	QString whatsThisTileSelectionPosterPage = i18n( " <qt> "
                        " <b>Tile Selection widget</b> "
			" <p>This GUI element is <em>not only for viewing</em> your selections: it also "
			" lets you interactively select the tile(s) you want to print. "
			" </p>"
			" <p><b>Hints</b> "
			" <ul> "
			" <li>Click any tile to select it for printing.</li> "
			" <li>To select multiple tiles to be printed "
			" at once, <em>'shift-click'</em> on the tiles ('shift-click' means: hold down the "
			" [SHIFT]-key on your keyboard and click with the mouse while [SHIFT]-key is held.)  "
			" <em>Be aware</em> that the order "
			" of your clicking is also significant to the order of printing the different tiles. </li>"
			" </ul> "
			" <b>Note 1:</b> The order of your selection (and the order for printout of the tiles) "
			" is indicated by the contents of the text field below, "
			" labelled as <em>'Tile pages (to be printed):'</em><p>"
			" <b>Note 2:</b> By default no tile is selected. Before you can print (a part "
			" of) your poster, you must select at least one tile. </p> "
			" </qt>" );

	QString whatsThisPostersizePosterPage = i18n( " <qt> "
                        " <b>Poster Size</b> "
			" <p>Select the poster size you want from the dropdown list. </p> "
			" Available sizes are all standard paper sizes up to "
			" 'A0'. [A0 is the same size as 16 sheets of A4, or '84cm x 118.2cm'.] </p> "
			" <p><b>Notice</b>, how the little preview window below changes with your change of poster "
			" size. It indicates to you how many tiles need to be printed to make the poster, "
			" given the selected paper size.</p>  "
			" <p><b>Hint:</b> The little preview window below is not just a passive icon. You can click "
			" on its individual tiles to select them for printing. To select multiple tiles to be printed "
			" at once, you need to <em>'shift-click'</em> on the tiles ('shift-click' means: hold down the "
			" [SHIFT]-key on your keyboard and click with the mouse while [SHIFT]-key is held.) The order "
			" of your clicking is significant to the order of printing the different tiles. The order of "
			" your selection (and for the printed tiles) is indicated by the contents of the text field "
			" labelled as <em>'Tile pages (to be printed):'</em><p>"
			" <b>Note:</b> By default no tile is selected. Before you can print (a part "
			" of) your poster, you must select at least one tile. </p> "
			" </qt>" );

	QString whatsThisPrintsizePosterPage = i18n( " <qt> "
                        " <b>Paper Size</b> "
			" <p>This field indicates the paper size the poster tiles will be printed on. "
			" To select a different paper size for your poster tiles, go to the 'General' tab "
			" of this dialog and select one from the dropdown list. </p> "
			" Available sizes are most standard paper sizes supported by your printer. Your printer's "
			" supported paper sizes are read from the printer driver info (as laid down in the <em>'PPD'</em>, "
			" the printer description file). <em>Be aware that the 'Paper Size' selected may not be supported "
			" by 'poster' (example: 'HalfLetter') while it may well be supported by your printer.</em>  If "
			" you hit that obstacle, simply use another, supported Paper Size, like 'A4' or 'Letter'. "
			" <p><b>Notice</b>, how the little preview window below changes with your change of paper "
			" size. It indicates how many tiles need to be printed to make up the poster, "
			" given the selected paper and poster size.</p>  "
			" <p><b>Hint:</b> The little preview window below is not just a passive icon. You can click "
			" on its individual tiles to select them for printing. To select multiple tiles to be printed "
			" at once, you need to <em>'shift-click'</em> on the tiles ('shift-click' means: hold down the "
			" [SHIFT]-key on your keyboard and click with the mouse while [SHIFT]-key is held.) The order "
			" of your clicking is significant to the order of printing the different tiles. The order of "
			" your selection (and for the printed tiles) is indicated by the contents of the text field "
			" labelled as <em>'Tile pages (to be printed):'</em><p>"
			" <b>Note:</b> By default no tile is selected. Before you can print (a part "
			" of) your poster, you must select at least one tile. </p> "
			" </qt>" );

	QString whatsThisCutmarginPosterPage = i18n( " <qt> "
                        " <b>Cut Margin selection</b> "
			" <p>Slider and spinbox let you determine a <em>'cut margin'</em> which will be printed onto "
			" each tile of your poster to help you cut the pieces as needed. </p>"
			" <p><b>Notice</b>, how the little preview window above changes with your change of cut "
			" margins. It indicates to you how much space the cut margins will take away from each tile. "
			" <p><b>Be aware</b>, that your cut margins need to be equal to or greater than the margins your "
			" printer uses. The printer's capabilities are described in the <em>'ImageableArea'</em> "
			" keywords of its driver PPD file. </p> "
			" </qt>" );

	QString whatsThisTileOrderSelectionPosterPage = i18n( " <qt> "
                        " <b>Order and number of tile pages to be printed</b> "
			" <p>This field displays and sets the individual tiles to be printed, as well as the order "
			" for their printout. </p> "
			" You can file the field with 2 different methods: "
			" <ul> "
			" <li>Either use the interactive thumbnail preview above and '[SHIFT]-click' on the tiles. </li> "
			" <li>Or edit this text field accordingly. </li> "
			" </ul> "
			" <p>When editing the field, you can use a '3-7' syntax instead of a '3,4,5,6,7' one. </p> "
			" <p><b>Examples:</b></p> "
			" <pre> "
			"   \"2,3,7,9,3\" "
			" <br> "
			"   \"1-3,6,8-11\" "
			" </qt>" );

    setWindowTitle( i18n( "Poster" ) );

	m_postercheck = new QCheckBox( i18n( "&Print poster" ), this );
    m_postercheck->setWhatsThis(whatsThisEnablePosterPage);
	QWidget *dummy = new QWidget( this );
	m_preview = new PosterPreview( dummy );
    m_preview->setMediaSize(QPrinter::A4);
    m_preview->setWhatsThis(whatsThisTileSelectionPosterPage);
	m_postersize = new QComboBox( dummy );
    m_postersize->setWhatsThis(whatsThisPostersizePosterPage);
	m_printsize = new QComboBox( dummy );
    m_printsize->setWhatsThis(whatsThisPrintsizePosterPage);
	m_lockbtn = new KPushButton( dummy );
          //QWhatsThis::add(m_lockbtn, whatsThis5_PosterPage);           //FIXME ASK_MICHAEL: which pushbutton would that be?
	m_mediasize = new QLabel( dummy );
    m_mediasize->setWhatsThis(whatsThisPrintsizePosterPage);
	m_mediasize->setFrameStyle( QFrame::Panel|QFrame::Sunken );
	QLabel *posterlab = new QLabel( i18n( "Poste&r size:" ), dummy );
    posterlab->setWhatsThis(whatsThisPostersizePosterPage);
	QLabel *medialab = new QLabel( i18n( "Media size:" ), dummy );
    medialab->setWhatsThis(whatsThisPrintsizePosterPage);
	QLabel *printlab = new QLabel( i18n( "Pri&nt size:" ), dummy );
    printlab->setWhatsThis(whatsThisPrintsizePosterPage);
	posterlab->setBuddy( m_postersize );
	printlab->setBuddy( m_printsize );
	m_cutmargin = new KIntNumInput( 5, dummy );
    m_cutmargin->setWhatsThis(whatsThisCutmarginPosterPage);
	// xgettext:no-c-format
	m_cutmargin->setLabel( i18n( "C&ut margin (% of media):" ) );
    m_cutmargin->setRange( 0, 100, 2 );                     // step width was too big, changed from 10 to 2 (-kp-)
    m_cutmargin->setSliderEnabled(true);
	m_selection = new QLineEdit( dummy );
    m_selection->setWhatsThis(whatsThisTileOrderSelectionPosterPage);
	QLabel *selectionlab = new QLabel( i18n( "&Tile pages (to be printed):" ), dummy );
    selectionlab->setWhatsThis(whatsThisTileOrderSelectionPosterPage);
	selectionlab->setBuddy( m_selection );
    m_lockbtn->setCheckable( true );
    m_lockbtn->setIcon(QIcon::fromTheme("document-encrypt"));
    m_lockbtn->setChecked( true );
	m_lockbtn->setFixedSize( m_lockbtn->sizeHint() );
    m_lockbtn->setToolTip(i18n( "Link/unlink poster and print size" ));

    QList<QPrinter::PaperSize> paperSizeList = PaperSizeUtils::allPaperSizes();
    Q_FOREACH (QPrinter::PaperSize paperSize, paperSizeList)
	{
        m_postersize->addItem( PaperSizeUtils::paperSizeToString(paperSize), paperSize );
        m_printsize->addItem( PaperSizeUtils::paperSizeToString(paperSize), paperSize );
    }
    m_postersize->setCurrentIndex( m_postersize->findData(QPrinter::A3) );
    slotPosterSizeChanged( m_postersize->currentIndex() );

    m_multipage_note_label = new QLabel(i18n("<i><b>Note:</b> Only first page will be taken for poster print.</i>"));
    m_multipage_note_label->setVisible( false );

    connect( m_postercheck, SIGNAL( toggled( bool ) ), dummy, SLOT( setEnabled( bool ) ) );
    dummy->setEnabled( false );
    connect( m_postercheck, SIGNAL( toggled(bool) ), SIGNAL( posterEnabled(bool) ) );
    connect( m_postersize, SIGNAL( activated( int ) ), SLOT( slotPosterSizeChanged( int ) ) );
    connect( m_printsize, SIGNAL( activated(int)), SLOT(slotPrintSizeChanged(int)));
    connect( m_cutmargin, SIGNAL( valueChanged( int ) ), SLOT( slotMarginChanged( int ) ) );
    connect( m_lockbtn, SIGNAL( toggled( bool ) ), m_printsize, SLOT( setDisabled( bool ) ) );
    m_printsize->setEnabled( false );
    connect( m_lockbtn, SIGNAL( toggled( bool ) ), SLOT( slotLockToggled( bool ) ) );
    connect( m_selection, SIGNAL( textEdited( const QString& ) ), m_preview, SLOT( setSelectedPages( const QString& ) ) );
    connect( m_preview, SIGNAL( selectionChanged( const QString& ) ), m_selection, SLOT( setText( const QString& ) ) );

    // TODO ???
    /*if ( KMFactory::self()->settings()->application != KPrinter::Dialog
            && KMFactory::self()->settings()->application >= 0 )
	{
		m_printsize->hide();
		m_lockbtn->hide();
		printlab->hide();
    }*/

    m_lockbtn->setChecked(false);
    m_lockbtn->hide();
    //m_printsize->setCurrentIndex( m_printsize->findData(QPrinter::A4) );

    QVBoxLayout *l0 = new QVBoxLayout( this );
    l0->setMargin(0);
    l0->setSpacing(10);
	l0->addWidget( m_postercheck );
	l0->addWidget( dummy, 1 );
    QGridLayout *l1 = new QGridLayout( dummy );
    l1->setMargin(0);
    l1->setSpacing(5);
	l1->addWidget( posterlab, 0, 0 );
	l1->addWidget( m_postersize, 0, 1 );
	l1->addWidget( printlab, 1, 0 );
	l1->addWidget( m_printsize, 1, 1 );
	l1->addWidget( medialab, 2, 0 );
	l1->addWidget( m_mediasize, 2, 1 );
    l1->addWidget( m_preview, 4, 0, 1, 3 );
    l1->addWidget( m_cutmargin, 6, 0, 1, 3 );
    l1->addWidget( m_lockbtn, 0, 2, 2, 1 );
    QHBoxLayout *l2 = new QHBoxLayout( 0 );
    l2->setMargin(0);
    l2->setSpacing(5);
    l1->addLayout( l2, 7, 0, 1, 3 );
	l2->addWidget( selectionlab );
	l2->addWidget( m_selection );
    l1->setColumnStretch( 1, 1 );
	l1->setRowStretch( 4, 1 );
    l1->addItem(new QSpacerItem(0, 10), 3, 0);
    l1->addItem(new QSpacerItem(0, 10), 5, 0);
    l1->addWidget( m_multipage_note_label, 8, 0, 1, 3 );

}

PosterWidget::~PosterWidget()
{
}

void PosterWidget::getOptions( QMap<QString,QString>& opts, bool )
{
    QStringList o = opts[ "_kde-filters" ].split(",", QString::SkipEmptyParts);
	if ( !m_postercheck->isChecked() )
	{
        o.removeAll( "poster" );
		opts[ "_kde-filters" ] = o.join( "," );
		opts.remove( "_kde-poster-media" );
		opts.remove( "_kde-poster-size" );
		opts.remove( "_kde-poster-cut" );
		opts.remove( "kde-printsize" );
		opts.remove( "_kde-poster-select" );
	}
	else
	{
		if ( !o.contains( "poster" ) )
			o.append( "poster" );
		opts[ "_kde-filters" ] = o.join( "," );
        opts[ "_kde-poster-media" ] = m_mediasize->text();
        opts[ "_kde-poster-size" ] = m_postersize->currentText(); //TODO: pageSizeToPageName( ( KPrinter::PageSize )page_sizes[ m_postersize->currentItem() ].ID );
        opts[ "kde-printsize" ] = m_printsize->currentText(); //QString::number( m_printsize->itemData(m_printsize->currentIndex()).toInt() );
		opts[ "_kde-poster-cut" ] = QString::number( m_cutmargin->value() );
        opts[ "_kde-poster-select" ] = m_selection->text().trimmed();
	}
}

bool PosterWidget::isEnabled()
{
    return m_postercheck->isChecked();
}

QSizeF PosterWidget::mediaSize() const
{
    // dummy function, needed for Q_PROPERTY
    return QSizeF();
}

void PosterWidget::setMediaSize(const QSizeF& mediaSizeFloat)
{
    QSize mediaSize = mediaSizeFloat.toSize();
    QPrinter::PageSize paperSize;

    if (mediaSize == QSize( 2384, 3370 )) {
        paperSize = QPrinter::A0;
    }
    else if (mediaSize == QSize( 1684, 2384 )) {
        paperSize = QPrinter::A1;
    }
    else if (mediaSize == QSize( 1191, 1684 )) {
        paperSize = QPrinter::A2;
    }
    else if (mediaSize == QSize(  842, 1191 )) {
        paperSize = QPrinter::A3;
    }
    else if (mediaSize == QSize(  595,  842 )) {
        paperSize = QPrinter::A4;
    }
    else if (mediaSize == QSize(  420,  595 )) {
        paperSize = QPrinter::A5;
    }
    else if (mediaSize == QSize(  298,  420 )) {
        paperSize = QPrinter::A6;
    }
    else if (mediaSize == QSize(  210,  298 )) {
        paperSize = QPrinter::A7;
    }
    else if (mediaSize == QSize(  147,  210 )) {
        paperSize = QPrinter::A8;
    }
    else if (mediaSize == QSize(  105,  147 )) {
        paperSize = QPrinter::A9;
    }

    else if (mediaSize == QSize( 2835, 4008 )) {
        paperSize = QPrinter::B0;
    }
    else if (mediaSize == QSize( 2004, 2835 )) {
        paperSize = QPrinter::B1;
    }
    else if (mediaSize == QSize( 1417, 2004 )) {
        paperSize = QPrinter::B2;
    }
    else if (mediaSize == QSize( 1001, 1417 )) {
        paperSize = QPrinter::B3;
    }
    else if (mediaSize == QSize(  709, 1001 )) {
        paperSize = QPrinter::B4;
    }
    else if (mediaSize == QSize(  499,  709 )) {
        paperSize = QPrinter::B5;
    }
    else if (mediaSize == QSize(  354,  499 )) {
        paperSize = QPrinter::B6;
    }
    else if (mediaSize == QSize(  249,  354 )) {
        paperSize = QPrinter::B7;
    }
    else if (mediaSize == QSize(  176,  249 )) {
        paperSize = QPrinter::B8;
    }
    else if (mediaSize == QSize(  125,  176 )) {
        paperSize = QPrinter::B9;
    }
    else if (mediaSize == QSize(   88,  125 )) {
        paperSize = QPrinter::B10;
    }

    else if (mediaSize == QSize(  459,  649 )) {
        paperSize = QPrinter::C5E;
    }
    else if (mediaSize == QSize(  297,  684 )) {
        paperSize = QPrinter::Comm10E;
    }
    else if (mediaSize == QSize(  312,  624 )) {
        paperSize = QPrinter::DLE;
    }
    else if (mediaSize == QSize(  522,  756 )) {
        paperSize = QPrinter::Executive;
    }
    else if (mediaSize == QSize(  595,  935 )) {
        paperSize = QPrinter::Folio;
    }
    else if (mediaSize == QSize( 1224,  792 )) {
        paperSize = QPrinter::Ledger;
    }
    else if (mediaSize == QSize(  612, 1008 )) {
        paperSize = QPrinter::Legal;
    }
    else if (mediaSize == QSize(  612,  792 )) {
        paperSize = QPrinter::Letter;
    }
    else if (mediaSize == QSize(  792, 1224 )) {
        paperSize = QPrinter::Tabloid;
    }
    else {
        paperSize = QPrinter::A4;
    }

    m_printsize->setCurrentIndex(m_printsize->findData(paperSize));

    // TODO proper
    int ID = m_printsize->itemData(m_printsize->currentIndex()).toInt();
    m_preview->setMediaSize( ID );
}

QString PosterWidget::mediaSizeDescription() const
{
    return m_mediasize->text();
}

void PosterWidget::setMediaSizeDescription(const QString& mediaSize)
{
    m_mediasize->setText(mediaSize.isEmpty() ? i18n("Unknown") : mediaSize);
}

void PosterWidget::showMultiPageNote(const bool show) {

  m_multipage_note_label->setVisible(show);

}

void PosterWidget::slotPosterSizeChanged( int value )
{
    int ID = m_postersize->itemData(m_postersize->currentIndex()).toInt();
	m_preview->setPosterSize( ID );
    if ( m_lockbtn->isChecked() )
        m_printsize->setCurrentIndex( value );
}

void PosterWidget::slotPrintSizeChanged( int )
{
    int ID = m_printsize->itemData(m_printsize->currentIndex()).toInt();
    m_preview->setMediaSize( ID );
}

void PosterWidget::slotMarginChanged( int value )
{
	m_preview->setCutMargin( value );
}

void PosterWidget::slotLockToggled( bool on )
{
    m_lockbtn->setIcon( QIcon::fromTheme( on ? "document-encrypt" : "document-decrypt" ) );
	if ( on )
        m_printsize->setCurrentIndex( m_postersize->currentIndex() );
}
