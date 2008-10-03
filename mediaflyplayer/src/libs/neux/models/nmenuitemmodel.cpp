/*
 *  Copyright(C) 2007 Neuros Technology International LLC. 
 *               <www.neurostechnology.com>
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
 ****************************************************************************
 *
 * MenuItemModel implement routines.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-27 SZ
 *
 */

#include "nmenuitemmodel.h"
#include "nlistview.h"

NMenuItemModel::NMenuItemModel (QWidget *parent, const QList<NMenuItem> &list)
: QAbstractListModel(parent), listEntries(list)
{
}

int NMenuItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

	return listEntries.size();
}

void NMenuItemModel::setListData(const QList<NMenuItem> &list) 
{
	 listEntries = list; 
	 reset();
}

bool NMenuItemModel::insertRows(const NMenuItem &item)
{
	listEntries.append(item);
	reset();
	return true;
}

bool NMenuItemModel::removeRows(int row)
{
	listEntries.removeAt(row);
	reset();
	return true;
}

bool NMenuItemModel::removeRows()
{
	listEntries.clear();
	reset();
	return true;
}

QVariant NMenuItemModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 )
        return QVariant();

	if ( listEntries.size() == 0 )
		return QVariant();

    if (role == NListView::CaptionRole )
    {
        return listEntries.at(index.row()).caption( );
    }
    else if (role == NListView::IconRole )
        return listEntries.at(index.row()).leftIcon ( );
    else if (role == NListView::RightIconRole )
        return listEntries.at(index.row()).rightIcon ( );
	else if (role == NListView::FgColorRole)
		return listEntries.at(index.row()).fgColor();
	else if (role == NListView::HLineRole)
		return listEntries.at(index.row()).hLine();
	return QVariant();
}


