/*
 *  Copyright(C) 2009 Milan Svoboda. 
 *               <milan.svoboda@centrum.cz>
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
 ****************************************************************************/

#ifndef mfList_H
#define mfList_H

#include "nribbonlistview.h"

#include <QObject>
#include <QListView>
#include <QKeyEvent>

namespace mf {

typedef NRibbonListView ListParent;

class List : public ListParent
{
	Q_OBJECT
public:
	List(QWidget *parent = 0);
	~List();

	void setEmpty() { m_empty = true; }
	void setItemDelegate(QAbstractItemDelegate *newDelegate);
	void keyPressEvent(QKeyEvent *event);

signals:
	void enterPressed();
	void leftPressed();
	void rightPressed();
	void exitPressed();

private:
	void paintEvent(QPaintEvent * e);

	bool m_empty;
};

}

#endif

