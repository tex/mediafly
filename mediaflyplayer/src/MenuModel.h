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

#ifndef mfMenuModel_H
#define mfMenuModel_H

#include "UsersModel.h"
#include <QStringList>
#include <QList>

namespace mf {

class MenuModel : public UsersModel
{
public:
	enum Roles
	{
		displayRole = Qt::DisplayRole,
		nameRole = Qt::UserRole + 2000,
		slugRole, 
		isUserRole,
		isDefaultRole,
	};

	enum Menus
	{
		MENU_SEARCH = 0,
		MENU_MEDIA_CHANNELS,
		MENU_POPULAR_CHANNELS,
		MENU_PERSONALIZE,
		MENU_USER,
		MENU_ADD_PERSON,
		MENU_FRIENDS_CHANNELS,
		MENU_REMOVE_PERSON,
	};

	MenuModel(QObject *parent = 0);

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& parent, int role) const;

private:
	QStringList m_name;
	QStringList m_name_users;
	QList<int>  m_slug;
	QList<int>  m_slug_users;
};

}

#endif

