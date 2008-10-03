#include "TMediafly.h"
#include "Mediafly.h"

#include <QtTest>
#include <iostream>

void TMediafly::testA()
{
	try {
		Mediafly mf("dfcfefff34d0458fa3df0e0c7a6feb6c", "N38r0s0sd");
		Mediafly::SessionInfo session = mf.Authentication_GetToken("123");
		QDomDocument doc = mf.Channels_GetChannels(session, false);
		qDebug() << doc.toString();
		QDomDocument doc1 = mf.Playlists_GetPlaylistForChannel(session, doc.firstChildElement("response").firstChildElement("channels").firstChildElement("channel").attribute("slug"), 0, 10, "audio,video");
		std::cout << doc1.toString().toStdString() << std::endl;
	}
	catch (Exception e) {
		QString errorMsg;
		e.getError(errorMsg);
		qDebug() << errorMsg;
	}
}

QTEST_MAIN(TMediafly)

