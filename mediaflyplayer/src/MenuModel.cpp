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

#include "MenuModel.h"

using namespace mf;

MenuModel::MenuModel(QObject *parent) :
	UsersModel(parent)
{
	m_name << tr("Search") << tr("Media Channels") /* << tr("Popular Channels") */ << tr("Personalize");
	m_slug << MENU_SEARCH << MENU_MEDIA_CHANNELS /* << MENU_POPULAR_CHANNELS */ << MENU_PERSONALIZE;

	m_name_users << tr("Search") /* << tr("Popular Channels") << tr("Friend's Channels") */ << tr("Personalize") << tr("Unbind active user");
	m_slug_users << MENU_SEARCH /* << MENU_POPULAR_CHANNELS << MENU_FRIENDS_CHANNELS */ << MENU_PERSONALIZE << MENU_REMOVE_PERSON;
}

int MenuModel::rowCount(const QModelIndex& parent) const
{
	int usersRowCount = UsersModel::rowCount(parent);
	if (usersRowCount == 0)
	{
		return m_name.size();
	}
	else
	{
		return m_name_users.size() + usersRowCount;
	}
}

QVariant MenuModel::data(const QModelIndex& parent, int role) const
{
	int rowCount = parent.row();
	int usersRowCount = UsersModel::rowCount(parent);

	if (usersRowCount == 0)
	{
		switch (role) {
		case displayRole: // Fall throught...
		case nameRole:
			return m_name.at(rowCount);
		case slugRole: return m_slug.at(rowCount);
		case isUserRole: return false;
		default:       return QVariant();
		}
	}
	else
	{
		if (rowCount < usersRowCount)
		{
			switch (role) {
			case displayRole: return UsersModel::data(createIndex(rowCount, 0), Qt::DisplayRole).toString();
			case nameRole: return UsersModel::data(createIndex(rowCount, 0), UsersModel::nameRole).toString();
			case isUserRole: return true;
			case isDefaultRole: return UsersModel::data(createIndex(rowCount, 0), UsersModel::isDefaultRole).toBool();
			case slugRole: return MENU_USER;
			default:       return QVariant();
			}
		}
		else
		{
			switch (role) {
			case displayRole: // Fall throught...
			case nameRole:
				return m_name_users.at(rowCount - usersRowCount);
			case slugRole: return m_slug_users.at(rowCount - usersRowCount);
			case isUserRole: return false;
			default:       return QVariant();
			}
		}
	}
}

