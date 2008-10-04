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
};

