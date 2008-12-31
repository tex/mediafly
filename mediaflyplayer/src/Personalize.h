#ifndef mfPersonalize_H
#define mfPersonalize_H

#include "MediaflyLoginPerson.h"
#include "ui_MediaflyPersonalize.h"

namespace mf {

class Personalize : public QWidget, private Ui::MediaflyPersonalize
{
	Q_OBJECT
public:
	Personalize(QWidget *parent = 0);
	void clear();

signals:
	void showLoginPerson();

private slots:
	void handleContinueButtonClick();
	void handleEnterButtonClick();
};

}

#endif

