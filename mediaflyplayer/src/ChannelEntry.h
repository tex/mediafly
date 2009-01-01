#ifndef mfChannelEntry_H
#define mfChannelEntry_H

#include <QString>
#include <QMetaType>

namespace mf {

class ChannelEntry
{
public:
	ChannelEntry()
	{
		qRegisterMetaType<ChannelEntry>("mf::ChannelEntry");
	}
	ChannelEntry(const ChannelEntry& obj) :
		m_name (obj.m_name),
		m_slug (obj.m_slug)
	{
	}

	ChannelEntry(QString name, QString slug) :
		m_name (name),
		m_slug (slug)
	{
		qRegisterMetaType<ChannelEntry>("mf::ChannelEntry");
	}

	const QString& name() const { return m_name; }
	const QString& slug() const { return m_slug; }

private:
	QString m_name;
	QString m_slug;
};

}

Q_DECLARE_METATYPE(mf::ChannelEntry);

#endif

