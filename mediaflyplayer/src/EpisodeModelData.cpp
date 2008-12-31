#include "EpisodeModelData.h"
#include <QDebug>

void mf::EpisodeModelData::read(const QDomDocument& doc)
{
	qDebug() << __PRETTY_FUNCTION__;

	QDomNode playlist = doc.firstChildElement("response").firstChildElement("playlist");

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

