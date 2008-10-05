#include "Mediafly.h"
#include "MediaflyChannelModelData.h"
#include <QNetworkInterface>

void MediaflyChannelModelData::readData()
{
	QNetworkInterface networkInterface = QNetworkInterface::interfaceFromName("eth0");
	QString hwAddress = networkInterface.hardwareAddress();

	Mediafly mf("dfcfefff34d0458fa3df0e0c7a6feb6c", "N38r0s0sd");
	Mediafly::SessionInfo session = mf.Authentication_GetToken(hwAddress);
	QDomDocument doc = mf.Channels_GetChannels(session, true);

	QDomNode it = doc.firstChildElement("response").firstChildElement("channels").firstChild();
	while (!it.isNull()) {
		QDomElement el = it.toElement();
		if (!el.isNull()) {
			MediaflyChannelEntry entry(el.attribute("name"), el.attribute("slug"));
			emit entryRead(entry);
		}
		it = it.nextSibling();
	}
}

