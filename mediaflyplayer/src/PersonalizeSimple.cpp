#include "PersonalizeSimple.h"
#include "Mediafly.h"

mf::PersonalizeSimple::PersonalizeSimple(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	// OK button emits back() signal to hide the widget...
	//
	connect(m_buttonOk, SIGNAL(clicked()),
	        this, SIGNAL(back()));

	// When we receive a code show it on the screen...
	//
	connect(&m_data, SIGNAL(userAssociationCodeRead(const QString&)),
	        m_labelCode, SLOT(setText(const QString&)));
}

void mf::PersonalizeSimple::show()
{
	m_labelCode->setText(tr("Retrieving..."));

	// Ask for a code...
	//
	Mediafly::getMediafly()->Authentication_GetUserAssociationCode(&m_data);
}

