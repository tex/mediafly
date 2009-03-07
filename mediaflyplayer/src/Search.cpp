#include "Search.h"

mf::Search::Search(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(m_cancelButton, SIGNAL(clicked(bool)), this, SIGNAL(back()));
	connect(m_okButton, SIGNAL(clicked(bool)), this, SLOT(handleOkButtonClicked()));
}

void mf::Search::show()
{
	m_lineEdit->clear();
}

void mf::Search::handleOkButtonClicked()
{
	// assemble search query
	//
	emit search(m_lineEdit->text());
}

