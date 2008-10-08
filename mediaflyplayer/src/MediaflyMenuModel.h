#ifndef MediaflyMenuModel_H
#define MediaflyMenuModel_H

#include <QAbstractListModel>
#include <QStringList>
#include <QList>

class MediaflyMenuModel : public QAbstractListModel
{
public:
	enum Roles
	{
		nameRole = Qt::DisplayRole,
		slugRole = Qt::UserRole,
	};

	enum Menus
	{
		MENU_SEARCH = 0,
		MENU_MEDIA_CHANNELS,
		MENU_POPULAR_CHANNELS,
		MENU_PERSONALIZE
	};

	MediaflyMenuModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& parent, int role) const;

private:
	QStringList m_name;
	QList<int>  m_slug;
};




#endif

