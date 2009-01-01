#include "List.h"
#include "MenuModel.h"
#include "MenuDelegate.h"
#include <QPainter>

using namespace mf;

List::List(QWidget *parent) :
	ListParent(parent)
{
	setItemDelegate(new mf::MenuDelegate());
}

List::~List()
{
}

void List::keyPressEvent(QKeyEvent *event)
{
	ListParent::keyPressEvent(event);

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

void List::paintEvent(QPaintEvent * e)
{
	if (!model() || (model() && model()->rowCount(rootIndex()) < 1))
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
		ListParent::paintEvent(e);
}
