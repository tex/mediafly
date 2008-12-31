#ifndef mfLoginPerson_H
#define mfLoginPerson_H

#include "BindMFUserData.h"
#include "ui_MediaflyLoginPerson.h"

namespace mf {

class LoginPerson : public QWidget, private Ui::MediaflyLoginPerson
{
	Q_OBJECT
public:
	LoginPerson(QWidget* parent = 0); 
	void clear();

signals:
	// Signals that new person has been signed in successfuly.
	//
	void newPerson();

	// Signals that user want's to leave the dialog.
	//
	void back();

private:
	auth::BindMFUserData m_data;

private slots:
	void handleAcceptButtonClick();
	void handleCancelButtonClick();
	void handleBindMFUserDone();
};

}

#endif

