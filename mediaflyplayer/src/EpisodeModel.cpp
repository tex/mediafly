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
	m_mediafly = Mediafly::getMediafly();
	m_refreshFinished = true;

	connect(&m_modelData, SIGNAL(entryRead(const mf::EpisodeEntry&)),
	        this, SLOT(handleEntryRead(const mf::EpisodeEntry&)));
	connect(&m_modelData, SIGNAL(entryReadFinished()),
	        this, SLOT(handleEntryReadFinished()));

	connect(&m_binaryData, SIGNAL(binaryRead(const QString&, const QByteArray&)),
	        this, SLOT(handleBinaryRead(const QString&, const QByteArray&)));
}

EpisodeModel::EpisodeModel(const EpisodeModel& obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_mediafly = Mediafly::getMediafly();
	m_data = obj.m_data;
	m_refreshFinished = obj.m_refreshFinished;
}

void EpisodeModel::clear()
{
	m_data.clear();
	m_modelData.clear();
	m_refreshFinished = true;
}

void EpisodeModel::refresh(const mf::EpisodeQuery& query)
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

void EpisodeModel::refresh()
{
	mf::EpisodeQuery query(m_query.channelSlug(), m_query.offset() + m_query.limit(), m_query.limit(), m_query.mediaType());
	refresh(query);
}

void EpisodeModel::refreshFull()
{
	clear();
	mf::EpisodeQuery query(m_query.channelSlug(), 0, m_query.offset() + m_query.limit(), m_query.mediaType());
	refresh(query);
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
	return m_modelData.totalEpisodes();
}

