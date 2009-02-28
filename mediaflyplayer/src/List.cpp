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

#include "List.h"
#include "MenuModel.h"
#include "MenuDelegate.h"
#include <QPainter>

using namespace mf;

List::List(QWidget *parent) :
	ListParent(parent),
	m_empty(false)
{
	setItemDelegate(new mf::MenuDelegate());
}

List::~List()
{
}

void List::keyPressEvent(QKeyEvent *event)
{
	ListParent::keyPressEvent(event);

	switch (event->key()) {
	case Qt::Key_Right:
		emit rightPressed();
		break;
	case Qt::Key_Left:
		m_empty = false;
		emit leftPressed();
		break;
	case Qt::Key_Down:
	case Qt::Key_PageDown:
	case Qt::Key_End:
		if (!model())
			break;
		if (currentIndex().row() + 15 > model()->rowCount())
			emit almostAtEndOfList();
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		emit enterPressed();
		break;
	default:
		event->ignore();
		break;
	}
}

void List::paintEvent(QPaintEvent * e)
{
	if (!model() || (model() && model()->rowCount(rootIndex()) < 1))
	{
		QPainter painter(viewport());
		QRect rect = painter.viewport();
		QString message;
		if (m_empty)
			message = tr("No items in menu");
		else
			message = tr("Loading menu, please wait...");
		int wMessage = painter.fontMetrics().width(message);
		int hMessage = painter.fontMetrics().height();
		int leftOffset = (rect.width() - wMessage) / 2;
		int topOffset = (rect.height() - hMessage) / 2;
		painter.drawText(leftOffset, topOffset, message);
	}
	else
		ListParent::paintEvent(e);
}

