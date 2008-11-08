#include "MediaflyMenuModel.h"

MediaflyMenuModel::MediaflyMenuModel(QObject *parent) :
	UsersModel(parent)
{
	m_name << tr("Search") << tr("Media Channels") << tr("Popular Channels") << tr("Personalize");
	m_slug << MENU_SEARCH << MENU_MEDIA_CHANNELS << MENU_POPULAR_CHANNELS << MENU_PERSONALIZE;

	m_name_users << tr("Search") << tr("Popular Channels") << tr("Friend's Channels") << tr("Add Person");
	m_slug_users << MENU_SEARCH << MENU_POPULAR_CHANNELS << MENU_FRIENDS_CHANNELS << MENU_ADD_PERSON;
}

int MediaflyMenuModel::rowCount(const QModelIndex& parent) const
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

QVariant MediaflyMenuModel::data(const QModelIndex& parent, int role) const
{
	int rowCount = parent.row();
	int usersRowCount = UsersModel::rowCount(parent);

	if (usersRowCount == 0)
	{
		switch (role) {
		case nameRole: return m_name.at(rowCount);
		case slugRole: return m_slug.at(rowCount);
		default:       return QVariant();
		}
	}
	else
	{
		if (rowCount < usersRowCount)
		{
			switch (role) {
			case nameRole:
			{
				QString name = UsersModel::data(createIndex(rowCount, 0), UsersModel::nameRole).toString() + QString(tr("'s Mediafly"));
				if (UsersModel::data(createIndex(rowCount, 0), UsersModel::defaultRole).toBool())
					name = "<b>" + name + "</b>";
				return name;
			}
			case origNameRole:
			{
				QString accountName = UsersModel::data(createIndex(rowCount, 0), UsersModel::nameRole).toString();
				qDebug() << __PRETTY_FUNCTION__ << "origNameRole:" <<  accountName;
				return accountName;
			}
			case defaultRole:
			{
				bool isDefault = UsersModel::data(createIndex(rowCount, 0), UsersModel::defaultRole).toBool();
				qDebug() << __PRETTY_FUNCTION__ << "isDefault:" << isDefault;
				return isDefault;
			}
			case slugRole: return MENU_USER;
			default:       return QVariant();
			}
		}
		else
		{
			switch (role) {
			case nameRole: return m_name_users.at(rowCount - usersRowCount);
			case slugRole: return m_slug_users.at(rowCount - usersRowCount);
			default:       return QVariant();
			}
		}
	}
}

