#include <QAbstractItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>

class MediaflyEpisodeDelegate : public QAbstractItemDelegate
{
public:
	MediaflyEpisodeDelegate(QObject *parent = 0) :
		QAbstractItemDelegate(parent) { }
	void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

