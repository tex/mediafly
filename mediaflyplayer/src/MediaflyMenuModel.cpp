#include "MediaflyMenuModel.h"

MediaflyMenuModel::MediaflyMenuModel()
{
	m_name << "Search" << "Media Channels" << "Popular Channels" << "Personalize";
	m_slug << MENU_SEARCH << MENU_MEDIA_CHANNELS << MENU_POPULAR_CHANNELS << MENU_PERSONALIZE;
}

int MediaflyMenuModel::rowCount(const QModelIndex& /*parent*/) const
{
	return m_name.size();
}

QVariant MediaflyMenuModel::data(const QModelIndex& parent, int role) const
{
	switch (role) {
	case nameRole: return m_name.at(parent.row());
	case slugRole: return m_slug.at(parent.row());
	default:       return QVariant();
	}
}

