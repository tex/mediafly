#ifndef MediaflyMenuModel_H
#define MediaflyMenuModel_H

#include "UsersModel.h"
#include <QStringList>
#include <QList>

using namespace mf;

class MediaflyMenuModel : public UsersModel
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

	MediaflyMenuModel(QObject *parent = 0);

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& parent, int role) const;

private:
	QStringList m_name;
	QStringList m_name_users;
	QList<int>  m_slug;
	QList<int>  m_slug_users;
};

#endif

