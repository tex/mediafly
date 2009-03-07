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

#ifndef mfEpisodeModelData_H
#define mfEpisodeModelData_H

#include "Consumer.h"
#include "EpisodeEntry.h"
#include <QDebug>

namespace mf {

class EpisodeModelData : public Consumer
{
	Q_OBJECT
public:
	EpisodeModelData()
	{
		m_totalEpisodes = -1;
		m_container = "playlist";
	}

	void read(const QDomDocument& doc);

	/**
	 * Returns total number of all episodes available.
	 * Return value valid after emmited entryRead() or entryReadFinished()
	 * signal.
	 */
	int totalEpisodes() const { return m_totalEpisodes; }

	void clear() { m_totalEpisodes = -1; }

signals:
	void entryRead(const mf::EpisodeEntry& entry);
	void entryReadFinished();

protected:
	int     m_totalEpisodes;
	QString m_container;
};

}

#endif

