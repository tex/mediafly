#ifndef MediaflyLoginPerson_H
#define MediaflyLoginPerson_H

#include "MediaflyAuthentication_BindMFUserData.h"
#include "ui_MediaflyLoginPerson.h"

class MediaflyLoginPerson : public QWidget, private Ui::MediaflyLoginPerson
{
	Q_OBJECT
public:
	MediaflyLoginPerson(QWidget* parent = 0); 
	void clear();

signals:
	// Signals that new person has been signed in successfuly.
	//
	void newPerson();

	// Signals that user want's to leave the dialog.
	//
	void back();

private:
	MediaflyAuthentication_BindMFUserData m_data;

private slots:
	void handleAcceptButtonClick();
	void handleCancelButtonClick();
	void handleBindMFUserDone();
};

#endif

