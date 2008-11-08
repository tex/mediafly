#ifndef User_H
#define User_H

#include <QString>
#include <QDebug>

namespace mf
{

class User
{
public:
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

