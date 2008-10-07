#include "MediaflyEpisodeModelData.h"
#include <QDebug>

void MediaflyEpisodeModelData::read(const QDomDocument& doc)
{
	qDebug() << __PRETTY_FUNCTION__;

	QDomNode it = doc.firstChildElement("response").firstChildElement("playlist").firstChild();
	while (!it.isNull()) {
		QDomElement el = it.toElement();
		if (!el.isNull()) {
			MediaflyEpisodeEntry entry(el.attribute("title"), el.attribute("slug"), el.attribute("description"),
			                           el.attribute("format"), el.attribute("url"), el.attribute("urlOriginal"),
			                           el.attribute("published"), el.attribute("showSlug"), el.attribute("showTitle"),
			                           el.attribute("imageUrl"), el.attribute("channel"));
			emit entryRead(entry);
		}
		it = it.nextSibling();
	}
	emit entryReadFinished();
}
/*
QByteArray MediaflyEpisodeModelData::readImage(const QString& imageUrl)
{
	if (imageUrl.isEmpty())
		return QByteArray();

	QUrl url(imageUrl);

	BHttp http;
	http.setHost(url.host());
	if (http.get(url.path()) == false)
		return QByteArray();
	return http.readAll();
}
*/

