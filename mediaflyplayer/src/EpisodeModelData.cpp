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

#include "EpisodeModelData.h"
#include <QDebug>

void mf::EpisodeModelData::read(const QDomDocument& doc)
{
	qDebug() << __PRETTY_FUNCTION__;

	QDomNode playlist = doc.firstChildElement("response").firstChildElement(m_container);

	m_totalEpisodes = playlist.toElement().attribute("totalEpisodes").toInt();

	QDomNode it = playlist.firstChild();
	while (!it.isNull()) {
		QDomElement el = it.toElement();
		if (!el.isNull()) {
			mf::EpisodeEntry entry(el.attribute("title"), el.attribute("slug"), el.attribute("description"),
			                       el.attribute("format"), el.attribute("url"), el.attribute("urlOriginal"),
			                       el.attribute("published"), el.attribute("showSlug"), el.attribute("showTitle"),
			                       el.attribute("imageUrl"), el.attribute("channel"));
			emit entryRead(entry);
		}
		it = it.nextSibling();
	}
	emit entryReadFinished();
}

