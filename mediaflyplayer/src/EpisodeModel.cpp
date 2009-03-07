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

#include "EpisodeModel.h"
#include <QDebug>
#include <QPixmapCache>

using namespace mf;

EpisodeModel::EpisodeModel(QObject *parent) :
	QAbstractListModel(parent)
{
	setState(EpisodeData);

	m_mediafly = Mediafly::getMediafly();
	m_refreshFinished = true;

	connect(&m_binaryData, SIGNAL(binaryRead(const QString&, const QByteArray&)),
	        this, SLOT(handleBinaryRead(const QString&, const QByteArray&)));
}

EpisodeModel::EpisodeModel(const EpisodeModel& obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_mediafly = Mediafly::getMediafly();
	m_state = obj.m_state;
	m_data = obj.m_data;
	m_refreshFinished = obj.m_refreshFinished;
}

void EpisodeModel::clear()
{
	m_data.clear();

	switch (m_state) {
	case EpisodeData:
		m_episodeModelData.clear();
		break;
	case SearchData:
		m_seachQueryData.clear();
		break;
	default:
		Q_ASSERT(false);
	}

	m_refreshFinished = true;
}

void EpisodeModel::setState(enum State state)
{
	if (m_state == state)
		return;

	m_state = state;

	switch (m_state) {
	case EpisodeData:
		disconnect(&m_seachQueryData, 0, this, 0);

		connect(&m_episodeModelData, SIGNAL(entryRead(const mf::EpisodeEntry&)),
		        this, SLOT(handleEntryRead(const mf::EpisodeEntry&)));
		connect(&m_episodeModelData, SIGNAL(entryReadFinished()),
		        this, SLOT(handleEntryReadFinished()));
		break;
	case SearchData:
		disconnect(&m_episodeModelData, 0, this, 0);

		connect(&m_seachQueryData, SIGNAL(entryRead(const mf::EpisodeEntry&)),
		        this, SLOT(handleEntryRead(const mf::EpisodeEntry&)));
		connect(&m_seachQueryData, SIGNAL(entryReadFinished()),
		        this, SLOT(handleEntryReadFinished()));
		break;
	default:
		Q_ASSERT(false);
	}
	clear();
}

void EpisodeModel::refresh(const mf::EpisodeQuery& query)
{
	setState(EpisodeData);

	if (!m_refreshFinished)
		return;
	m_refreshFinished = false;

	if ((m_data.size() < m_episodeModelData.totalEpisodes()) ||	// Not all episodes loaded yet.
	    (m_episodeModelData.totalEpisodes() == -1))		// Unknown number of episodes.
	{
		m_mediafly->Playlists_GetPlaylistForChannel(&m_episodeModelData, query);
		m_episodeQuery = query;
	}
}

void EpisodeModel::refresh(const mf::SearchQuery& query)
{
	setState(SearchData);

	if (!m_refreshFinished)
		return;
	m_refreshFinished = false;

	if ((m_data.size() < m_seachQueryData.totalEpisodes()) ||	// Not all episodes loaded yet.
	    (m_seachQueryData.totalEpisodes() == -1))			// Unknown number of episodes.
	{
		m_mediafly->Search_Query(&m_seachQueryData, query);
		m_searchQuery = query;
	}
}

void EpisodeModel::refresh()
{
	switch (m_state) {
	case EpisodeData:
		m_episodeQuery.advanceOffset();
		refresh(m_episodeQuery);
		break;
	case SearchData:
		m_searchQuery.advanceOffset();
		refresh(m_searchQuery);
		break;
	default:
		Q_ASSERT(false);
	}
}

void EpisodeModel::refreshFull()
{
	clear();

	switch (m_state) {
	case EpisodeData:
		m_episodeQuery.clearOffset();
		refresh(m_episodeQuery);
		break;
	case SearchData:
		m_searchQuery.clearOffset();
		refresh(m_searchQuery);
		break;
	default:
		Q_ASSERT(false);
	}
}

void EpisodeModel::cancel()
{
	m_mediafly->abort();
}

void EpisodeModel::handleEntryRead(const mf::EpisodeEntry& entry)
{
	qDebug() << __PRETTY_FUNCTION__ << entry.title();

	m_data[m_data.size()] = entry;
	emit refreshed();
}

void EpisodeModel::handleEntryReadFinished()
{
	qDebug() << __PRETTY_FUNCTION__;

	m_refreshFinished = true;
	emit refreshed();
}

void EpisodeModel::handleBinaryRead(const QString& path, const QByteArray& array)
{
	QPixmap image; image.loadFromData(array);
	QPixmapCache::insert(path, image.scaled(80, 80));

	emit refreshed();
}

int EpisodeModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_data.size();
}

QVariant EpisodeModel::data(const QModelIndex &index, int role) const
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
			m_mediafly->Utility_GetImage(const_cast<mf::ConsumerBinary *>(&m_binaryData), path);
		}
		if (role == imageUrlRole)
			return path;
		return image;
	} 
	case channelRole:     return (m_data[index.row()]).channel();
	default:              return QVariant();
	}
}

int EpisodeModel::totalRowCount() const
{
	switch (m_state) {
	case EpisodeData:
		return m_episodeModelData.totalEpisodes();
	case SearchData:
		return m_seachQueryData.totalEpisodes();
	default:
		Q_ASSERT(false);
	}
	// We newer reach this code. Just make compiler happy.
	// 
	return -1;
}

bool EpisodeModel::advanceToNextEpisode(QModelIndex& index)
{
	if (index.row() + 15 > index.model()->rowCount())
		dynamic_cast<EpisodeModel *>(
			const_cast<QAbstractItemModel *>(index.model())
		)->refresh();
	if (index.row() + 1 < index.model()->rowCount()) {
		index = index.model()->index(index.row() + 1, 0);
		return true;
	}
	return false;
}

bool EpisodeModel::advanceToPreviousEpisode(QModelIndex& index)
{
	if (index.row() > 0) {
		index = index.model()->index(index.row() - 1, 0);
		return true;
	}
	return false;
}

