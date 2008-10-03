#ifndef MLISTVIEW_H
#define MLISTVIEW_H

#include <QListView>
#include <QKeyEvent>

typedef QListView MParentView;

class MListView : public MParentView
{
	Q_OBJECT
public:
	MListView(QWidget *parent = 0) : MParentView(parent) { }
	~MListView() { }

	void keyPressEvent(QKeyEvent *event) {
		MParentView::keyPressEvent(event);
		if (event->key() == Qt::Key_Right)
			emit rightKeyPressed(currentIndex());
		else if (event->key() == Qt::Key_Left)
			emit leftKeyPressed(currentIndex());
		else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
			emit enterKeyPressed(currentIndex());
		else if (event->key() == Qt::Key_Down)
			emit downKeyPressed(currentIndex());
	}
signals:
	void enterKeyPressed(const QModelIndex &index);
	void rightKeyPressed(const QModelIndex &index);
	void leftKeyPressed(const QModelIndex &index);
	void downKeyPressed(const QModelIndex &index);
};

#endif

