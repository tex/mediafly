#ifndef mfEpisodeQuery_H
#define mfEpisodeQuery_H

#include <QString>

namespace mf {

class EpisodeQuery
{
public:
	EpisodeQuery() { }

	EpisodeQuery(QString channelSlug, int offset, int limit, QString mediaType = "audio,video") :
		m_channelSlug(channelSlug),
		m_offset(offset),
		m_limit(limit),
		m_mediaType(mediaType)
	{ }

	EpisodeQuery(const EpisodeQuery& obj) :
		m_channelSlug(obj.m_channelSlug),
		m_offset(obj.m_offset),
		m_limit(obj.m_limit),
		m_mediaType(obj.m_mediaType)
	{ }

	const QString& channelSlug() const { return m_channelSlug; }
	int offset() const { return m_offset; }
	int limit() const { return m_limit; }
	const QString& mediaType() const { return m_mediaType; }

	void setChannelSlug(QString channelSlug) { m_channelSlug = channelSlug; }
private:
	QString m_channelSlug;
	int     m_offset;
	int     m_limit;
	QString m_mediaType;
};

}

#endif
