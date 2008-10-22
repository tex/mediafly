/*
 *  Copyright(C) 2007 Neuros Technology International LLC. 
 *               <www.neurostechnology.com>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *  
 *
 *  This program is distributed in the hope that, in addition to its 
 *  original purpose to support Neuros hardware, it will be useful 
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************
 *
 * NRibbonListView implement routies.
 *
 * REVISION:
 * 
 * 2) Rename this widget and add fade effect to NRibbonListView. ------------ 2008-03-04 NW
 * 1) Initial creation. ----------------------------------- 2007-11-20 EY
 *
 */
#include <QtGui>

#include "nribbonlistview.h"
#include "nmenuitemmodel.h"
#include "nitemdelegate.h"

#define MIN_LIGHTNESS_FACTOR    250

NRibbonListView::NRibbonListView(QWidget *parent)
: NListView(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_InputMethodEnabled,false);
    setShowLeftArrowIcon( false );
	movementPolicy = DefaultSelectionMovementPolicy;
	pageItems = -1;
}

void NRibbonListView::showEvent( QShowEvent * event )
{
	if(model() == 0)
		return;
	setCurrentIndex ( model( )->index ( 0 , 0 ,rootIndex( )) );
}

void NRibbonListView::resizeEvent(QResizeEvent *e)
{
    NListView::resizeEvent(e);
	pageItems = -1;
	pageItemAccount( );
}

void NRibbonListView::reset()
{
	NListView::reset();
	pageItems = -1;
}

void NRibbonListView::pageItemAccount()
{
	if (model() == 0)
		return ;

	midItemRect = NListView::visualRect(model()->index(0,0,rootIndex( ) ) ) ;

    if ( !midItemRect.isValid( ))
        return;

    pageItems = viewport()->height( ) / midItemRect.height( );

    if ( pageItems > model()->rowCount(rootIndex()))
    {
        pageItems = model()->rowCount(rootIndex());
    }

	if (movementPolicy == CursorStayBelowMiddle)
	{
		midItemRect.moveTop((viewport()->height()-midItemRect.height())/2);
		viewportRect = QRect ( midItemRect.x() , midItemRect.y() - midItemRect.height( ) *
							   (pageItems / 2) , midItemRect.width( ) , 
							   midItemRect.height( ) * pageItems );
    }
	else if(movementPolicy == CursorStayAboveMiddle)
	{
		midItemRect.moveTop((viewport()->height()-midItemRect.height())/2);
		viewportRect = QRect ( midItemRect.x() , midItemRect.y() - midItemRect.height( ) *
							   ( (pageItems + 1) / 2 - 1 ) , midItemRect.width( ) , 
							   midItemRect.height( ) * pageItems );
    }
	else
	{
		viewportRect = QRect ( midItemRect.x() , midItemRect.y() , midItemRect.width( ) ,
							   midItemRect.height( ) * pageItems );
	}
  
}

void NRibbonListView :: onAnimationFrameChanged(int frame)
{
	bPaintAnimation = true;
	QRect currRect = midItemRect;
	viewport()->update(QRect(captionOffset(), captionSize()).translated(currRect.left(), 
																		currRect.top() + currRect.height() /2 
																		- captionSize().height()/2));
}

void NRibbonListView :: paintEvent ( QPaintEvent *e )
{
	if (pageItems < 0)
		pageItemAccount();
	
	if (model() == 0)
		return NListView::paintEvent(e);
    /* call delegate to paint */
     QStyleOptionViewItem option = viewOptions();
     QPainter painter(viewport( ));
     
     int row = currentIndex( ).row( );
     int rowCnt = model()->rowCount(rootIndex());

     if (rowCnt == 0) 
         return NListView :: paintEvent ( e );

    if (bPaintAnimation && (QTimeLine::Running == animation.state()))
    {
        painter.setPen( Qt::white);
        option.state |= QStyle::State_HasFocus;
        option.rect = midItemRect;
        paintAnimation(&painter, option, e->rect());
        bPaintAnimation = false;
        return;
    }

	QPalette curPalette = palette();
	QColor baseTextColor = curPalette.color(QPalette::Normal,QPalette::WindowText);
	QColor minLightnessTextColor = baseTextColor.darker(MIN_LIGHTNESS_FACTOR);
	int curIndex = 0;
	if( movementPolicy == CursorStayBelowMiddle && pageItems > 1 )
		curIndex = pageItems/2;
	else
	{
		if( (pageItems > 1) && ((pageItems-1)>>1) )
			curIndex = ((pageItems-1)>>1);
	}
	int colorCount = pageItems>>1;

	NItemDelegate* pNItemDelegate = (NItemDelegate *)itemDelegate();
	for( int i=0; i<pageItems && i<rowCnt; i++ )
	{
		QModelIndex index = model()->index((rowCnt+row+i-curIndex)%rowCnt, 0, rootIndex());
		if( currentIndex ( ) == index )
			option.state |= QStyle::State_HasFocus;
		else
			option.state &= ~QStyle::State_HasFocus;
		option.rect = midItemRect;

		if( i == curIndex )
		{
			painter.setPen( baseTextColor );
		}
		else
		{
			painter.setPen( baseTextColor.darker(MIN_LIGHTNESS_FACTOR*100/(MIN_LIGHTNESS_FACTOR-(MIN_LIGHTNESS_FACTOR-100)*abs(i-curIndex)/colorCount)) );
			option.rect.moveTo( midItemRect.x(), midItemRect.y( ) + (i-curIndex) * midItemRect.height( ));
		}

		if (!viewport( )->rect( ).contains(option.rect.adjusted(1, 1, -1, -1), true))
		{
			continue;
		}

		pNItemDelegate->paint(&painter,option,index);
	}
}

QModelIndex NRibbonListView :: moveCursor ( CursorAction cursorAction, 
									  Qt::KeyboardModifiers modifiers )
{
    viewport()->update(viewportRect );
    return NListView :: moveCursor ( cursorAction , modifiers);
}

