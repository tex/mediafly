#include "MediaflyPersonalize.h"

MediaflyPersonalize::MediaflyPersonalize(MediaflyLoginPerson* loginPerson, QWidget *parent) :
	QWidget(parent),
	m_loginPerson(loginPerson)
{
	setupUi(this);

	connect(m_ContinueButton, SIGNAL(clicked()),
	        this, SLOT(handleContinueButtonClick()));
	connect(m_EnterButton, SIGNAL(clicked()),
	        this, SLOT(handleEnterButtonClick()));
}

void MediaflyPersonalize::show()
{
	m_stackedWidget->setCurrentIndex(0);
	QWidget::show();
}

void MediaflyPersonalize::handleContinueButtonClick()
{
	m_stackedWidget->setCurrentIndex(1);
}

void MediaflyPersonalize::handleEnterButtonClick()
{
	hide();
	m_loginPerson->show();
}

