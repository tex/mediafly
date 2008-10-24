#ifndef MediaflyEpisodeDelegate_H
#define MediaflyEpisodeDelegate_H

#include <QItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>

class MediaflyEpisodeDelegate : public QItemDelegate
{
public:
	MediaflyEpisodeDelegate(QObject *parent = 0) :
		QItemDelegate(parent) { }
	void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
private:
	static int vMargin;
	static int hMargin;
	static int showLines;
	static QSize iconSize;
};

#endif

