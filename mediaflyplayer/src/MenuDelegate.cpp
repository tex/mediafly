#include "MenuDelegate.h"
#include "MenuModel.h"

void mf::MenuDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{

	const QString label = index.data(Qt::DisplayRole).toString();
	const bool isDefault = index.data(mf::MenuModel::isDefaultRole).toBool();

	if (option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());
		painter->setPen(option.palette.highlightedText().color());
	} else
		painter->setPen(option.palette.text().color());

	QFont font(painter->font());
	if (isDefault) {
		font.setWeight(QFont::Bold);
	} else
		font.setWeight(QFont::Normal);
	painter->setFont(font);

	painter->drawText(option.rect, label);
}

