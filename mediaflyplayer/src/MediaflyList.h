#include <QObject>
#include <QWidget>
#include <QListView>
#include <QKeyEvent>
#include "nribbonlistview.h"

//typedef NRibbonListView MediaflyListParent;
typedef QListView MediaflyListParent;

class MediaflyList : public MediaflyListParent
{
	Q_OBJECT
public:
	MediaflyList(QWidget *parent = 0);
	~MediaflyList();

signals:
	void almostAtEndOfList();
	void enterPressed();
	void leftPressed();
	void rightPressed();

private:
	void keyPressEvent(QKeyEvent *event);
};

