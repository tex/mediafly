#ifndef MediaflyMenuDelegate_H
#define MediaflyMenuDelegate_H

#include <QItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>

class MediaflyMenuDelegate : public QItemDelegate
{
public:
	MediaflyMenuDelegate(QWidget *parent = 0) :
		QItemDelegate(parent) { }
	void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

#endif

