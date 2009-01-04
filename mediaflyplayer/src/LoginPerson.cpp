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

#include "LoginPerson.h"
#include "Mediafly.h"
#include "nmessagebox.h"
#include "nhelpbox.h"

using namespace mf;

LoginPerson::LoginPerson(QWidget* parent) :
	NBackgroundManagedWidget(parent)
{
	setupUi(this);
	m_password->setToolTip(tr("Enter password"));
	m_username->setToolTip(tr("Enter username"));

	setPreferredBackground(BackgroundVideoOnly);

	connect(&m_data, SIGNAL(done()),
	        this, SLOT(handleBindMFUserDone()));
}

void LoginPerson::clear()
{
	m_password->clear();
	m_username->clear();
}

void LoginPerson::handleBindMFUserDone()
{
	NMessageBox::information(this, tr("Congratulations!"),
	                         tr("Your new Mediafly account link has been successfully added to the Mediafly main menu"),
	                         QMessageBox::Ok, QMessageBox::Ok, 5 * 1000);
	emit newPerson();
}

void LoginPerson::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);

	switch(event->key()) {
	case Qt::Key_Right:
	case Qt::Key_Enter:
	case Qt::Key_Return:
		Mediafly::getMediafly()->Authentication_BindMFUser(&m_data, m_username->text(), m_password->text());
		m_status->setText(tr("Request send, please wait..."));
		break;
	case Qt::Key_Left:
	case Qt::Key_Escape:
	case Qt::Key_Back:
		emit back();
		break;
	case Qt::Key_Up:
	case Qt::Key_PageUp:
		m_username->setFocus();
		break;
	case Qt::Key_Down:
	case Qt::Key_PageDown:
		m_password->setFocus();
		break;
	case Qt::Key_Help:
		NHelpBox::NHelpBoxNew(tr("Possible keys"),
		                      tr("Left - Back to main menu\n") +
		                      tr("Right - Send informations to the Mediafly server\n") +
		                      tr("Up - Set focus on username edit box\n") +
		                      tr("Down - Set focus on password edit box\n"));
		break;
	default:
		event->ignore();
		break;
	}
}

