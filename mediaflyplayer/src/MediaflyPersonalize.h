#include "MediaflyLoginPerson.h"
#include "ui_MediaflyPersonalize.h"

class MediaflyPersonalize : public QWidget, private Ui::MediaflyPersonalize
{
	Q_OBJECT
public:
	MediaflyPersonalize(MediaflyLoginPerson* loginPerson, QWidget *parent = 0);
	void show();

private slots:
	void handleContinueButtonClick();
	void handleEnterButtonClick();

protected:
	MediaflyLoginPerson* m_loginPerson;
};

