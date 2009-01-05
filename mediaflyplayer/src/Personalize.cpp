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

#include "Personalize.h"
#include <QKeyEvent>
#include "nhelpbox.h"

using namespace mf;

Personalize::Personalize(QWidget *parent) :
	NBackgroundManagedWidget(parent)
{
	setupUi(this);

	setPreferredBackground(BackgroundVideoOnly);
}

void Personalize::clear()
{
	m_stackedWidget->setCurrentIndex(0);
}

void Personalize::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);

	switch (event->key()) {
	case Qt::Key_Right:
	case Qt::Key_Return:
	case Qt::Key_Enter:
		if (m_stackedWidget->currentIndex() == m_stackedWidget->count() - 1)
			emit showLoginPerson();
		else
			m_stackedWidget->setCurrentIndex(
				m_stackedWidget->currentIndex() + 1
			);
		break;
	case Qt::Key_Left:
	case Qt::Key_Escape:
		if (m_stackedWidget->currentIndex() == 0)
			emit hide();
		else
			m_stackedWidget->setCurrentIndex(
				m_stackedWidget->currentIndex() - 1
			);
		break;
	case Qt::Key_Help:
		switch (m_stackedWidget->currentIndex()) {
		case 0:
			NHelpBox::NHelpBoxNew(tr("Possible keys"),
			                      tr("Left - Back to main menu\n") +
			                      tr("Right - To next screen\n"));
			break;
		case 1:
			NHelpBox::NHelpBoxNew(tr("Possible keys"),
			                      tr("Left - To the previous screen\n") +
			                      tr("Right - To enter username and password\n"));
			break;
		}
		break;
	}
}

