#ifndef MediaflyChannelEntry_H
#define MediaflyChannelEntry_H

#include <QString>
#include <QMetaType>

class MediaflyChannelEntry
{
public:
	MediaflyChannelEntry()
	{
		qRegisterMetaType<MediaflyChannelEntry>("MediaflyChannelEntry");
	}
	MediaflyChannelEntry(const MediaflyChannelEntry& obj) :
		m_name (obj.m_name),
		m_slug (obj.m_slug)
	{
		qRegisterMetaType<MediaflyChannelEntry>("MediaflyChannelEntry");
	}

	MediaflyChannelEntry(QString name, QString slug) :
		m_name (name),
		m_slug (slug)
	{
		qRegisterMetaType<MediaflyChannelEntry>("MediaflyChannelEntry");
	}

	const QString& name() const { return m_name; }
	const QString& slug() const { return m_slug; }

private:
	QString m_name;
	QString m_slug;
};

Q_DECLARE_METATYPE(MediaflyChannelEntry);

#endif

