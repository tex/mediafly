#include "EpisodeDelegate.h"
#include "EpisodeModel.h"

using namespace mf;

int   EpisodeDelegate::vMargin = 5;
int   EpisodeDelegate::hMargin = 5;
int   EpisodeDelegate::showLines = 3;
QSize EpisodeDelegate::iconSize = QSize(50, 50);

void EpisodeDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());
		painter->setPen(option.palette.highlightedText().color());
	} else
		painter->setPen(option.palette.text().color());

	QPixmap icon(index.data(mf::EpisodeModel::imageRole).value<QPixmap>());
	QString episodeTitle(index.data(mf::EpisodeModel::titleRole).toString());
	QString showTitle(index.data(mf::EpisodeModel::showTitleRole).toString());

	QRect r = option.rect.adjusted(2, 2, -2, -2);
	int hText = option.fontMetrics.height();

	QRect i(QPoint(r.left()                   , r.top() + (r.height() - iconSize.height()) / 2),
	        QPoint(r.left() + iconSize.width(), r.height()));

	QRect t1(QPoint(r.left() + i.width() + hMargin , r.top()),
	         QPoint(r.width() - i.width() - hMargin, r.top() + hText * 2));

	QRect t2(QPoint(r.left() + i.width() + hMargin , r.top() + hText * 2),
	         QPoint(r.width() - i.width() - hMargin, r.height() - vMargin));

	painter->drawPixmap(i, icon.scaled(iconSize.width(), iconSize.height()));
	painter->drawText(t1, Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap, episodeTitle);
	painter->drawText(t2, Qt::AlignBottom|Qt::AlignLeft|Qt::TextWordWrap, showTitle);
}

QSize EpisodeDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
{
	// Return size big enought to hold a 'showLines' lines of text plus
	// some spacing on top and bottom.
	//
	return QSize(1, option.fontMetrics.height() * showLines + vMargin * 2);
}

