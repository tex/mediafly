#include "Personalize.h"

using namespace mf;

Personalize::Personalize(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(m_ContinueButton, SIGNAL(clicked()),
	        this, SLOT(handleContinueButtonClick()));
	connect(m_EnterButton, SIGNAL(clicked()),
	        this, SLOT(handleEnterButtonClick()));
}

void Personalize::clear()
{
	m_stackedWidget->setCurrentIndex(0);
}

void Personalize::handleContinueButtonClick()
{
	m_stackedWidget->setCurrentIndex(1);
}

void Personalize::handleEnterButtonClick()
{
	emit showLoginPerson();
}

