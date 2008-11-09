#include "MediaflyPersonalize.h"

MediaflyPersonalize::MediaflyPersonalize(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(m_ContinueButton, SIGNAL(clicked()),
	        this, SLOT(handleContinueButtonClick()));
	connect(m_EnterButton, SIGNAL(clicked()),
	        this, SLOT(handleEnterButtonClick()));
}

void MediaflyPersonalize::clear()
{
	m_stackedWidget->setCurrentIndex(0);
}

void MediaflyPersonalize::handleContinueButtonClick()
{
	m_stackedWidget->setCurrentIndex(1);
}

void MediaflyPersonalize::handleEnterButtonClick()
{
	emit showLoginPerson();
}

