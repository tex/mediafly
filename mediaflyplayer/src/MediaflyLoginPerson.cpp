#include "MediaflyLoginPerson.h"
#include "Mediafly.h"
#include <QMessageBox>

MediaflyLoginPerson::MediaflyLoginPerson(QWidget* parent) :
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

void MediaflyLoginPerson::handleAcceptButtonClick()
{
	Mediafly::getMediafly()->Authentication_BindMFUser(&m_data, m_username->text(), m_password->text());
}

void MediaflyLoginPerson::clear()
{
	m_password->clear();
	m_username->clear();
	m_username->setFocus();
}

void MediaflyLoginPerson::handleCancelButtonClick()
{
	emit back();
}

void MediaflyLoginPerson::handleBindMFUserDone()
{
	QMessageBox::information(this, tr("Congratilations!"),
	                         tr("Your new Mediafly account link has been successfully added to the Mediafly main menu"));
	emit newPerson();
}

