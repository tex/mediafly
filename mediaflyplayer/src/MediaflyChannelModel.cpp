#include "Mediafly.h"
#include "MediaflyChannelModel.h"
#include <iostream>
MediaflyChannelModel::MediaflyChannelModel(QObject *parent) :
	QAbstractListModel(parent)
{
	qRegisterMetaType<MediaflyChannelModel>("MediaflyChannelModel");
}

MediaflyChannelModel::MediaflyChannelModel(const MediaflyChannelModel &obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_nameList = obj.m_nameList;
	m_slugList = obj.m_slugList;
}

void MediaflyChannelModel::refresh()
{
	Mediafly mf("dfcfefff34d0458fa3df0e0c7a6feb6c", "N38r0s0sd");
	Mediafly::SessionInfo session = mf.Authentication_GetToken("123");
	QDomDocument doc = mf.Channels_GetChannels(session, true);

	QDomNode it = doc.firstChildElement("response").firstChildElement("channels").firstChild();
	while (!it.isNull()) {
		QDomElement el = it.toElement();
		if (!el.isNull()) {
			m_nameList << el.attribute("name");
			m_slugList << el.attribute("slug");
		}
		it = it.nextSibling();
	}
}

int MediaflyChannelModel::rowCount(const QModelIndex &/*parent*/) const
{
	Q_ASSERT(m_nameList.size() == m_slugList.size());

	return m_nameList.size();
}

QVariant MediaflyChannelModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	Q_ASSERT(m_nameList.size() == m_slugList.size());

	if (index.row() >= m_nameList.size())
		return QVariant();

	if (role == nameRole)
		return m_nameList.at(index.row());
	else if (role == slugRole)
		return m_slugList.at(index.row());
	else
		return QVariant();
}

