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

#ifndef UsersModel_H
#define UsersModel_H

#include "User.h"
#include "UsersModel.h"
#include "Mediafly.h"
#include "GetBoundMFUsersData.h"
#include <QAbstractListModel>
#include <QList>

namespace mf
{

class UsersModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles
	{
		displayRole = Qt::DisplayRole,
		nameRole = Qt::UserRole + 1000,
		passwordRole, 
		isDefaultRole,
	};

	UsersModel(QObject *parent = 0);
	UsersModel(const UsersModel &obj);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

	QString getDefaultAccountName() const;

signals:
	void refreshed();

public slots:
	void refresh();
	void cancel();

private:
	Mediafly                                  *m_mediafly;
	auth::GetBoundMFUsersData                  m_modelData;
	QList<User>                                m_data;
	int                                        m_id;

private slots:
	void handleUserRead(const mf::User& user);
	void handleUserFinished();
};

}

#endif

