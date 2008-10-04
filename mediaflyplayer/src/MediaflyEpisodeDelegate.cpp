#include "MediaflyEpisodeDelegate.h"
#include "MediaflyEpisodeModel.h"

void MediaflyEpisodeDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (option.state & QStyle::State_Selected)
	{
		painter->fillRect(option.rect, option.palette.highlight());
		painter->setPen(option.palette.highlightedText().color());
	}
	else
	{
		painter->setPen(option.palette.text().color());
	}

//	QIcon icon = QIcon(qvariant_cast<QPixmap>(index.data(MediaflyEpisodeModel::imageRole)));
	QString icon = index.data(MediaflyEpisodeModel::imageUrlRole).toString();
	QString episodeTitle = index.data(MediaflyEpisodeModel::titleRole).toString();
	QString showTitle = index.data(MediaflyEpisodeModel::showTitleRole).toString();

	QRect r = option.rect.adjusted(2, 2, -2, -2);

	painter->drawText(r.left(), r.top(), 20, 20, Qt::AlignVCenter|Qt::AlignHCenter, icon);
	painter->drawText(r.left() + 25, r.top(), r.width() - 25, r.height() / 2, Qt::AlignVCenter|Qt::AlignLeft|Qt::TextWordWrap, episodeTitle);
	painter->drawText(r.left() + 25, r.top() + r.height() / 2, r.width() - 25, r.height() / 2, Qt::AlignVCenter|Qt::AlignLeft|Qt::TextWordWrap, showTitle);
}

QSize MediaflyEpisodeDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSize size = QItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() * 2);
	return size;
}

