#ifndef UsersModel_H
#define UsersModel_H

#include "User.h"
#include "UsersModel.h"
#include "Mediafly.h"
#include "MediaflyAuthentication_GetBoundMFUsersData.h"
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
		nameRole = Qt::DisplayRole,
		passwordRole = Qt::UserRole + 1000,
		isDefaultRole,
	};

	UsersModel(QObject *parent = 0);
	UsersModel(const UsersModel &obj);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

signals:
	void refreshed();

public slots:
	void refresh();
	void cancel();

private:
	Mediafly                                  *m_mediafly;
	MediaflyAuthentication_GetBoundMFUsersData m_modelData;
	QList<User>                                m_data;
	int                                        m_id;

private slots:
	void handleUserRead(const mf::User& user);
	void handleUserFinished();
};

}

#endif

