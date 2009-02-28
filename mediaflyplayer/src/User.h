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

#ifndef User_H
#define User_H

#include <QString>
#include <QDebug>

namespace mf
{

class User
{
public:
	User() {}
	User(QString accountName, bool isDefault = false, QString password = "") :
		m_accountName(accountName),
		m_password(password),
		m_isDefault(isDefault)
	{
		qDebug() << "new User(" << m_accountName << m_password << m_isDefault << ")";
	}

	const QString& getAccountName() const { return m_accountName; }
	const QString& getPassword() const { return m_password; }
	bool isDefault() const { return m_isDefault; }

private:
	QString m_accountName;
	QString m_password;
	bool    m_isDefault;
};

}

#endif

