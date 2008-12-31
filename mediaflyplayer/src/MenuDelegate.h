#ifndef mfMenuDelegate_H
#define mfMenuDelegate_H

#include <QItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>

namespace mf {

class MenuDelegate : public QItemDelegate
{
public:
	MenuDelegate(QWidget *parent = 0) :
		QItemDelegate(parent) { }

	void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

}

#endif

