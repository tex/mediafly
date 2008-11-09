#include "MediaflyMenuDelegate.h"
#include "MediaflyMenuModel.h"

void MediaflyMenuDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QString label(index.data(MediaflyMenuModel::nameRole).toString());
	bool isUserRole = index.data(MediaflyMenuModel::isUserRole).toBool();
	bool isDefault = index.data(MediaflyMenuModel::isDefaultRole).toBool();

	if (option.state & QStyle::State_Selected) {
		painter->fillRect(option.rect, option.palette.highlight());
		painter->setPen(option.palette.highlightedText().color());
	} else
		painter->setPen(option.palette.text().color());

	if (isUserRole)
		label += tr("'s Mediafly");

	QFont font(painter->font());
	if (isDefault) {
		font.setWeight(QFont::Bold);
	} else
		font.setWeight(QFont::Normal);
	painter->setFont(font);

	painter->drawText(option.rect, label);
}

