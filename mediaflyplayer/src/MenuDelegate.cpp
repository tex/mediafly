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

	NItemDelegate::paint(painter, option, index);
}

