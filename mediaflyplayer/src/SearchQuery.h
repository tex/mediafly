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

#ifndef mfSearchQuery_H
#define mfSearchQuery_H

#include "Query.h"
#include <QString>

namespace mf {

class SearchQuery : public Query
{
public:
	SearchQuery() { }

	SearchQuery(QString term, QString searchType = "episode", QString explicitFilter = "notexplicit", QString mediaType = "audio,video", int offset = 0, int limit = 10, bool includeCounts = true) :
		Query(offset, limit),
		m_term(term),
		m_searchType(searchType),
		m_explicitFilter(explicitFilter),
		m_mediaType(mediaType),
		m_includeCounts(includeCounts)
	{ }

	SearchQuery(const SearchQuery& obj) :
		Query(obj.m_offset, obj.m_limit),
		m_term(obj.m_term),
		m_searchType(obj.m_searchType),
		m_explicitFilter(obj.m_explicitFilter),
		m_mediaType(obj.m_mediaType),
		m_includeCounts(obj.m_includeCounts)
	{ }

	QString term() const { return m_term; }
	QString searchType() const { return m_searchType; }
	QString explicitFilter() const { return m_explicitFilter; }
	QString mediaType() const { return m_mediaType; }
	bool includeCounts() const { return m_includeCounts; }

private:
	QString m_term;
	QString m_searchType;
	QString m_explicitFilter;
	QString m_mediaType;
	bool    m_includeCounts;
};

}

#endif

