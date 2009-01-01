#ifndef mfList_H
#define mfList_H

#include <QObject>
#include <QWidget>
#include <QListView>
#include <QKeyEvent>

namespace mf {

typedef QListView ListParent;

class List : public ListParent
{
	Q_OBJECT
public:
	List(QWidget *parent = 0);
	~List();

signals:
	void almostAtEndOfList();
	void enterPressed();
	void leftPressed();
	void rightPressed();

private:
	void keyPressEvent(QKeyEvent *event);
	void paintEvent(QPaintEvent * e);
};

}

#endif

