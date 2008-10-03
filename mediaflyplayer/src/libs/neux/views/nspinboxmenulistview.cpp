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
 * NSpinBoxMenuListView implement routines.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-23 WX
 *
 */

#include <QtGui>

#include "nspinboxmenulistview.h"

#define BACKGROUND_COLOR  QColor(0x10, 0x10, 0x10, 255)
#define GRAT_COLOR QColor(0x7a,0x7a,0x7a,255)

NSpinBoxMenuListView::NSpinBoxMenuListView(QWidget *parent)
:NRibbonListView(parent)
{
}

void NSpinBoxMenuListView::resizeEvent(QResizeEvent *event)
{
    NRibbonListView::resizeEvent(event);
	if (currentIndex().row() < 0)
	{
		setCurrentIndex(model()->index(0, 0, rootIndex()));
	}
}

void NSpinBoxMenuListView::paintEvent(QPaintEvent *event)
{
	if (this->hasFocus())
    {
		NRibbonListView::paintEvent(event);
	}
	else
	{
        QStyleOptionViewItem option = viewOptions();
        QPainter painter(viewport( ));
        option.rect = midItemRect;
        option.state |= QStyle::State_HasFocus;
        itemDelegate()->paint(&painter , option ,  currentIndex());
	}
}
