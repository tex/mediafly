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

mf::Search::Search(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(m_cancelButton, SIGNAL(clicked(bool)), this, SIGNAL(back()));
	connect(m_okButton, SIGNAL(clicked(bool)), this, SLOT(handleOkButtonClicked()));
}

void mf::Search::show()
{
	m_lineEdit->clear();
	m_lineEdit->setFocus();
}

void mf::Search::handleOkButtonClicked()
{
	// assemble search query
	//
	emit search(m_lineEdit->text());
}

