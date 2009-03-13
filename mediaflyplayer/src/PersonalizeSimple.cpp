#include "PersonalizeSimple.h"
#include "Mediafly.h"
#include <QKeyEvent>

mf::PersonalizeSimple::PersonalizeSimple(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

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

void mf::PersonalizeSimple::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Escape:
	case Qt::Key_Left:
		emit back();
		break;
	}
}

