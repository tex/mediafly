#include "MediaflyList.h"
#include <QPainter>

MediaflyList::MediaflyList(QWidget *parent) :
	MediaflyListParent(parent)
{
}

MediaflyList::~MediaflyList()
{
}

void MediaflyList::keyPressEvent(QKeyEvent *event)
{
	QListView::keyPressEvent(event);

	switch (event->key()) {
	case Qt::Key_Right:
		emit rightPressed();
		break;
	case Qt::Key_Left:
		emit leftPressed();
		break;
	case Qt::Key_Down:
	case Qt::Key_PageDown:
	case Qt::Key_End:
		if (!model())
			break;
		if (currentIndex().row() + 15 > model()->rowCount())
			emit almostAtEndOfList();
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		emit enterPressed();
		break;
	default:
		event->ignore();
		break;
	}
}

void MediaflyList::paintEvent(QPaintEvent * e)
{
	if (model() && model()->rowCount(rootIndex()) < 1)
	{
		QPainter painter(viewport());
		QRect rect = painter.viewport();
		QString message = tr("Loading menu, please wait...");
		int wMessage = painter.fontMetrics().width(message);
		int hMessage = painter.fontMetrics().height();
		int leftOffset = (rect.width() - wMessage) / 2;
		int topOffset = (rect.height() - hMessage) / 2;
		painter.drawText(leftOffset, topOffset, message);
	}
	else
		MediaflyListParent::paintEvent(e);
}

