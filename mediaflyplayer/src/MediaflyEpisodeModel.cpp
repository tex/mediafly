#include "MediaflyEpisodeModel.h"
#include <QDebug>

MediaflyEpisodeModel::MediaflyEpisodeModel(QObject *parent) :
	QAbstractListModel(parent)
{
	m_mediafly = Mediafly::getMediafly();

	connect(&m_modelData, SIGNAL(entryRead(const MediaflyEpisodeEntry&)),
	        this, SLOT(handleEntryRead(const MediaflyEpisodeEntry&)));

	connect(&m_binaryData, SIGNAL(binaryRead(const QByteArray&)),
	        this, SLOT(handleBinaryRead(const QByteArray&)));
}

MediaflyEpisodeModel::MediaflyEpisodeModel(const MediaflyEpisodeModel& obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_mediafly = Mediafly::getMediafly();
	m_data = obj.m_data;
	m_image = obj.m_image;
}

void MediaflyEpisodeModel::clear()
{
	m_data.clear();
	m_image.clear();
}

void MediaflyEpisodeModel::refresh(const MediaflyEpisodeQuery& query)
{
	if (m_data.size() < m_modelData.totalEpisodes()) {
		m_mediafly->Playlists_GetPlaylistForChannel(&m_modelData, query);
	}
}

void MediaflyEpisodeModel::cancel()
{
	m_mediafly->abort();
}

void MediaflyEpisodeModel::handleEntryRead(const MediaflyEpisodeEntry& entry)
{
	qDebug() << __PRETTY_FUNCTION__ << entry.title();

	if (entry.imageUrl() != "") {
		qDebug() << "LOADING IMAGE";
		m_mediafly->Utility_GetImage(&m_binaryData, entry.imageUrl());
	}

	m_data[m_data.size()] = entry;
	emit refreshed();
}

void MediaflyEpisodeModel::handleBinaryRead(const QByteArray& buffer)
{
	qDebug() << __PRETTY_FUNCTION__;

	m_image[m_image.size()] = buffer;
	emit refreshed();
}

int MediaflyEpisodeModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_data.size();
}

QVariant MediaflyEpisodeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_data.size())
		return QVariant();

	switch (role) {
	case titleRole:       return (m_data[index.row()]).title();
	case slugRole:        return (m_data[index.row()]).slug();
	case descriptionRole: return (m_data[index.row()]).description();
	case formatRole:      return (m_data[index.row()]).format();
	case urlRole:         return (m_data[index.row()]).url();
	case urlOriginalRole: return (m_data[index.row()]).urlOriginal();
	case publishedRole:   return (m_data[index.row()]).published();
	case showSlugRole:    return (m_data[index.row()]).showSlug();
	case showTitleRole:   return (m_data[index.row()]).showTitle();
	case imageUrlRole:    return (m_data[index.row()]).imageUrl();
	case imageRole:       return (m_image[index.row()]);
	case channelRole:     return (m_data[index.row()]).channel();
	default:              return QVariant();
	}
}

