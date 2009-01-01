#include "ChannelModelData.h"
#include <QDebug>

void mf::ChannelModelData::read(const QDomDocument& doc)
{
	qDebug() << __PRETTY_FUNCTION__;

	QDomNode it = doc.firstChildElement("response").firstChildElement("channels").firstChild();
	while (!it.isNull()) {
		QDomElement el = it.toElement();
		if (!el.isNull()) {
			mf::ChannelEntry entry(el.attribute("name"), el.attribute("slug"));
			emit entryRead(entry);
		}
		it = it.nextSibling();
	}
	emit entryReadFinished();
}

