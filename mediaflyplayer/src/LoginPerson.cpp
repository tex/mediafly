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

using namespace mf;

LoginPerson::LoginPerson(QWidget* parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(m_accept, SIGNAL(clicked()),
	        this, SLOT(handleAcceptButtonClick()));
	connect(m_cancel, SIGNAL(clicked()),
	        this, SLOT(handleCancelButtonClick()));

	connect(&m_data, SIGNAL(done()),
	        this, SLOT(handleBindMFUserDone()));
}

void LoginPerson::handleAcceptButtonClick()
{
	Mediafly::getMediafly()->Authentication_BindMFUser(&m_data, m_username->text(), m_password->text());
}

void LoginPerson::clear()
{
	m_password->clear();
	m_username->clear();
	m_username->setFocus();
}

void LoginPerson::handleCancelButtonClick()
{
	emit back();
}

void LoginPerson::handleBindMFUserDone()
{
	NMessageBox::information(this, tr("Congratulations!"),
	                         tr("Your new Mediafly account link has been successfully added to the Mediafly main menu"),
	                         QMessageBox::Ok, QMessageBox::Ok, 5 * 1000);
	emit newPerson();
}

