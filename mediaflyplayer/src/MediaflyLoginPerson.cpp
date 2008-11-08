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

void MediaflyLoginPerson::show()
{
	m_username->clear();
	m_password->clear();
	QWidget::show();
}

void MediaflyLoginPerson::handleCancelButtonClick()
{
	hide();
}

void MediaflyLoginPerson::handleBindMFUserDone()
{
	// Binding of MF user succeeded.
	//
	emit newPerson();

	QMessageBox::information(this, tr("Congratilations!"),
	                         tr("Your new Mediafly account link has been successfully added to the Mediafly main menu"));
	hide();
}

