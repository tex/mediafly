#include "BHttp.h"
#include "REST.h"
#include "Mediafly.h"
#include "MediaflyEpisodeModelData.h"
#include <QNetworkInterface>
#include <QDomDocument>
#include <QByteArray>
#include <QUrl>

void MediaflyEpisodeModelData::readData(QString channelSlug, int offset, int limit, QString mediaType)
{
	QNetworkInterface networkInterface = QNetworkInterface::interfaceFromName("eth0");
	QString hwAddress = networkInterface.hardwareAddress();

	Mediafly mf("dfcfefff34d0458fa3df0e0c7a6feb6c", "N38r0s0sd");
	Mediafly::SessionInfo session = mf.Authentication_GetToken(hwAddress);
	QDomDocument doc = mf.Playlists_GetPlaylistForChannel(session, channelSlug, offset, limit, mediaType);

	QDomNode it = doc.firstChildElement("response").firstChildElement("playlist").firstChild();
	while (!it.isNull()) {
		QDomElement el = it.toElement();
		if (!el.isNull()) {
			MediaflyEpisodeEntry entry(el.attribute("title"), el.attribute("slug"), el.attribute("description"),
			                           el.attribute("format"), el.attribute("url"), el.attribute("urlOriginal"),
			                           el.attribute("published"), el.attribute("showSlug"), el.attribute("showTitle"),
			                           el.attribute("imageUrl"), el.attribute("channel"));
			emit entryRead(entry);
			emit imageRead(entry.imageUrl());
		}
		it = it.nextSibling();
	}
}

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

