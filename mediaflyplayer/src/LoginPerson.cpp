#include "LoginPerson.h"
#include "Mediafly.h"
#include <QMessageBox>

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
	QMessageBox::information(this, tr("Congratulations!"),
	                         tr("Your new Mediafly account link has been successfully added to the Mediafly main menu"));
	emit newPerson();
}

