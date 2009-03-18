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

#ifndef mfEpisodeQuery_H
#define mfEpisodeQuery_H

#include "Query.h"
#include <QString>

namespace mf {

class EpisodeQuery : public Query
{
public:
	EpisodeQuery() { }

	EpisodeQuery(QString channelSlug, int offset, int limit, QString mediaType = "audio,video") :
		Query(offset, limit),
		m_channelSlug(channelSlug),
		m_mediaType(mediaType)
	{ }

	EpisodeQuery(const EpisodeQuery& obj) :
		Query(obj.m_offset, obj.m_limit)
	{
		if (this == &obj)
			return;
		m_channelSlug = obj.m_channelSlug;
		m_mediaType = obj.m_mediaType;
	}

	const QString& channelSlug() const { return m_channelSlug; }
	const QString& mediaType() const { return m_mediaType; }

	void setChannelSlug(QString channelSlug) { m_channelSlug = channelSlug; }
private:
	QString m_channelSlug;
	QString m_mediaType;
};

}

#endif

