#include "MediaflyEpisodeModel.h"
#include <QDebug>
#include <QPixmapCache>

MediaflyEpisodeModel::MediaflyEpisodeModel(QObject *parent) :
	QAbstractListModel(parent)
{
	m_mediafly = Mediafly::getMediafly();
	m_refreshFinished = true;

	connect(&m_modelData, SIGNAL(entryRead(const MediaflyEpisodeEntry&)),
	        this, SLOT(handleEntryRead(const MediaflyEpisodeEntry&)));
	connect(&m_modelData, SIGNAL(entryReadFinished()),
	        this, SLOT(handleEntryReadFinished()));

	connect(&m_binaryData, SIGNAL(binaryRead(const QString&, const QByteArray&)),
	        this, SLOT(handleBinaryRead(const QString&, const QByteArray&)));
}

MediaflyEpisodeModel::MediaflyEpisodeModel(const MediaflyEpisodeModel& obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_mediafly = Mediafly::getMediafly();
	m_data = obj.m_data;
	m_refreshFinished = obj.m_refreshFinished;
}

void MediaflyEpisodeModel::clear()
{
	m_data.clear();
	m_modelData.clear();
	m_refreshFinished = true;
}

void MediaflyEpisodeModel::refresh(const MediaflyEpisodeQuery& query)
{
	if (!m_refreshFinished)
		return;
	m_refreshFinished = false;

	if ((m_data.size() < m_modelData.totalEpisodes()) ||	// Not all episodes loaded yet.
	    (m_modelData.totalEpisodes() == -1))		// Unknown number of episodes.
	{
		m_mediafly->Playlists_GetPlaylistForChannel(&m_modelData, query);
		m_query = query;
	}
}

void MediaflyEpisodeModel::refresh()
{
	MediaflyEpisodeQuery query(m_query.channelSlug(), m_query.offset() + m_query.limit(), m_query.limit(), m_query.mediaType());
	refresh(query);
}

void MediaflyEpisodeModel::refreshFull()
{
	clear();
	MediaflyEpisodeQuery query(m_query.channelSlug(), 0, m_query.offset() + m_query.limit(), m_query.mediaType());
	refresh(query);
}

void MediaflyEpisodeModel::cancel()
{
	m_mediafly->abort();
}

void MediaflyEpisodeModel::handleEntryRead(const MediaflyEpisodeEntry& entry)
{
	qDebug() << __PRETTY_FUNCTION__ << entry.title();

	m_data[m_data.size()] = entry;
	emit refreshed();
}

void MediaflyEpisodeModel::handleEntryReadFinished()
{
	m_refreshFinished = true;
}

void MediaflyEpisodeModel::handleBinaryRead(const QString& path, const QByteArray& array)
{
	QPixmap image; image.loadFromData(array);
	QPixmapCache::insert(path, image.scaled(80, 80));

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
	case imageUrlRole: 
	case imageRole:
	{
		QString path = m_data[index.row()].imageUrl();
		QPixmap image;
		if (!QPixmapCache::find(path, image)) {
			m_mediafly->Utility_GetImage(const_cast<MediaflyConsumerBinary *>(&m_binaryData), path);
		}
		if (role == imageUrlRole)
			return path;
		return image;
	} 
	case channelRole:     return (m_data[index.row()]).channel();
	default:              return QVariant();
	}
}

int MediaflyEpisodeModel::totalRowCount() const
{
	return m_modelData.totalEpisodes();
}

