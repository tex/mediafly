/*
 *  Copyright(C) 2009 Milan Svoboda. 
 *               <milan.svoboda@centrum.cz>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *  
 *
 *  This program is distributed in the hope that, in addition to its 
 *  original purpose to support Neuros hardware, it will be useful 
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************/

#include "EpisodeDelegate.h"
#include "EpisodeModel.h"

using namespace mf;

int   EpisodeDelegate::vMargin = 5;
int   EpisodeDelegate::hMargin = 5;
int   EpisodeDelegate::showLines = 3;

EpisodeDelegate::EpisodeDelegate(QWidget *parent) :
	NItemDelegate(parent)
{
	QFontMetrics fm(parent->font());

	// Return size big enought to hold a 'showLines' lines of text plus
	// some spacing on top and bottom.

	ItemHintSize = QSize(1, fm.height() * showLines + vMargin * 2);
}

void EpisodeDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, bool active) const
{
	QColor color = index.data(NListView::FgColorRole).value<QColor>(); 
	if (color.isValid())
	{
		painter->setPen(color);
	}
	else
	{
		painter->setPen(active ? ACTIVE_COLOR : INACTIVE_COLOR);
	}

	QString episodeTitle(index.data(mf::EpisodeModel::titleRole).toString());
	QString showTitle(index.data(mf::EpisodeModel::showTitleRole).toString());

	int hText = option.fontMetrics.height();

	QPoint pt11(option.rect.x() + CaptionPosition.x(), option.rect.y() + CaptionPosition.y() + vMargin);
	QPoint pt12(pt11.x() + CaptionSize.width(), pt11.y() + hText * 2);

	QRect t1(pt11, pt12);

	QPoint pt21(option.rect.x() + CaptionPosition.x(), pt11.y() + hText * 2);
	QPoint pt22(pt21.x() + CaptionSize.width(), pt21.y() + hText);

	QRect t2(pt21, pt22);

	painter->drawText(t1, Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap, episodeTitle);
	painter->drawText(t2, Qt::AlignBottom|Qt::AlignLeft|Qt::TextWordWrap, showTitle);
}

QSize EpisodeDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
{
	return ItemHintSize;
}

