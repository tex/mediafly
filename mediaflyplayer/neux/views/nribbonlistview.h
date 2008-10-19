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
 * NRibbonListView implement header.
 *
 * REVISION:
 * 
 * 2) Rename this widget and add fade effect to NRibbonListView. ------------ 2008-03-04 NW
 * 1) Initial creation. ----------------------------------- 2007-11-20 EY
 *
 */

#ifndef NRIBBONLISTVIEW_H
#define NRIBBONLISTVIEW_H


#include "nlistview.h"

class NRibbonListView : public NListView
{
    Q_OBJECT
public:
	enum SelectionMovementPolicy
	{
		CursorStayAboveMiddle,
		CursorStayBelowMiddle,
        CursorStayOnTop,
		DefaultSelectionMovementPolicy = CursorStayAboveMiddle,
	};
	NRibbonListView(QWidget *parent = 0);
	void setSelectionMovementPolicy(const SelectionMovementPolicy policy){movementPolicy = policy;}
	const SelectionMovementPolicy selectionMovementPolicy() const {return movementPolicy;}

protected:
	void showEvent( QShowEvent * event );
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    QModelIndex moveCursor ( CursorAction cursorAction, 
							 Qt::KeyboardModifiers modifiers );
public slots:
	void reset();
protected slots:
    void onAnimationFrameChanged(int frame);

protected:
    QRect midItemRect;

private:
	void pageItemAccount();
    QRect viewportRect;
    int pageItems;

	SelectionMovementPolicy movementPolicy;
};


#endif // NRIBBONLISTVIEW_H

