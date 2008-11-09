#include "MediaflyLoginPerson.h"
#include "ui_MediaflyPersonalize.h"

class MediaflyPersonalize : public QWidget, private Ui::MediaflyPersonalize
{
	Q_OBJECT
public:
	MediaflyPersonalize(QWidget *parent = 0);
	void clear();

signals:
	void showLoginPerson();

private slots:
	void handleContinueButtonClick();
	void handleEnterButtonClick();
};

