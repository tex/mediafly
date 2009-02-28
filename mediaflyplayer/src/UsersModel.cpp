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

#include "UsersModel.h"

using namespace mf;

UsersModel::UsersModel(QObject *parent) :
	QAbstractListModel(parent)
{
	m_mediafly = Mediafly::getMediafly();

	connect(&m_modelData, SIGNAL(userRead(const mf::User&)),
	        this, SLOT(handleUserRead(const mf::User&)));

	connect(&m_modelData, SIGNAL(userReadFinished()),
	        this, SLOT(handleUserFinished()));
}

UsersModel::UsersModel(const UsersModel &obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_mediafly = Mediafly::getMediafly();
	m_data = obj.m_data;
}

void UsersModel::cancel()
{
	// TODO
}

void UsersModel::refresh()
{
	m_id = 0;
	m_mediafly->Authentication_GetBoundMFUsers(&m_modelData);
}

void UsersModel::handleUserRead(const mf::User& user)
{
	qDebug() << user.getAccountName() << user.getPassword() << user.isDefault();

	if (m_data.size() > m_id)
		m_data.replace(m_id, user);
	else
		m_data.insert(m_id, user);
	++m_id;
}

void UsersModel::handleUserFinished()
{
	for (int i = m_id; i < m_data.size(); ++i) {
		m_data.removeAt(i);
	}
	emit refreshed();
}

int UsersModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_data.size();
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_data.size())
		return QVariant();

	switch (role) {
	case displayRole: return m_data.at(index.row()).getAccountName() + tr("'s Mediafly");
	case nameRole: return m_data.at(index.row()).getAccountName();
	case passwordRole: return m_data.at(index.row()).getPassword();
	case isDefaultRole:
	{
		bool isDefault = m_data.at(index.row()).isDefault();
		qDebug() << __PRETTY_FUNCTION__ << "isDefault:" << isDefault;
		return isDefault;
	}
	default:       return QVariant();
	}
}

QString UsersModel::getDefaultAccountName() const
{
	mf::User user;
	foreach(user, m_data) {
		if (user.isDefault())
			return user.getAccountName();
	}
	return QString();
}

