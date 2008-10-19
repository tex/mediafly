#ifndef _NMENUITEMMODEL_H__
#define _NMENUITEMMODEL_H__
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
 * NMenuItemModel implement header.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-27 SZ
 *
 */
#include <QWidget>
#include <QAbstractItemView>
#include <QList>
#include "nmenuitem.h"

class NMenuItemModel :  public QAbstractListModel
{
public:
	explicit NMenuItemModel (QWidget *parent = 0,  const QList<NMenuItem> &list = QList<NMenuItem>());

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

	bool insertRows(const NMenuItem &item);
    bool removeRows(int row);
	bool removeRows();

	void setListData(const QList<NMenuItem> &list);

private:
	QList<NMenuItem> listEntries;
};

#endif 
