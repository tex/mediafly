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

#include "Search.h"
#include "nhelpbox.h"

mf::Search::Search(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
	m_lineEdit->setToolTip(tr("Enter term"));

	connect(m_lineEdit, SIGNAL(editingFinished()),
	        this, SLOT(handleEditingFinished()));
}

void mf::Search::clear()
{
	m_lineEdit->clear();
	m_lineEdit->setFocus();
}

void mf::Search::handleEditingFinished()
{
	if (m_lineEdit->text().isEmpty())
	{
		emit back();
	} else
		emit search(m_lineEdit->text());
}

void mf::Search::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Escape:
		emit back();
		break;
	}
}

