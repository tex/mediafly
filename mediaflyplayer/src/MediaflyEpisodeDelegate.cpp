#include "MediaflyEpisodeDelegate.h"
#include "MediaflyEpisodeModel.h"
#include <QDebug>

void MediaflyEpisodeDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());
		painter->setPen(option.palette.highlightedText().color());
	} else
		painter->setPen(option.palette.text().color());

	QPixmap icon;
	icon.loadFromData(index.data(MediaflyEpisodeModel::imageRole).toByteArray());
	QString episodeTitle(index.data(MediaflyEpisodeModel::titleRole).toString());
	QString showTitle(index.data(MediaflyEpisodeModel::showTitleRole).toString());

	QRect r = option.rect.adjusted(2, 2, -2, -2);
	QRect i(r.left(), r.top(), 40, 40);

	painter->drawPixmap(i, icon.scaled(i.width(), i.height()));
	painter->drawText(r.left() + i.width() + 5, r.top(), r.width() - i.width() - 5, r.height() / 2, Qt::AlignVCenter|Qt::AlignLeft|Qt::TextWordWrap, episodeTitle);
	painter->drawText(r.left() + i.width() + 5, r.top() + r.height() / 2, r.width() - i.width() - 5, r.height() / 2, Qt::AlignVCenter|Qt::AlignLeft|Qt::TextWordWrap, showTitle);
}

QSize MediaflyEpisodeDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSize size = QItemDelegate::sizeHint(option, index);
	size.setHeight(50);
	return size;
}

