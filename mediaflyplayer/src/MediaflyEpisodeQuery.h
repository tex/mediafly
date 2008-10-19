#ifndef MediaflyEpisodeQuery_H
#define MediaflyEpisodeQuery_H

#include <QString>

class MediaflyEpisodeQuery
{
public:
	MediaflyEpisodeQuery() { }

	MediaflyEpisodeQuery(QString channelSlug, int offset, int limit, QString mediaType = "audio,video") :
		m_channelSlug(channelSlug),
		m_offset(offset),
		m_limit(limit),
		m_mediaType(mediaType)
	{ }

	MediaflyEpisodeQuery(const MediaflyEpisodeQuery& obj) :
		m_channelSlug(obj.m_channelSlug),
		m_offset(obj.m_offset),
		m_limit(obj.m_limit),
		m_mediaType(obj.m_mediaType)
	{ }

	const QString& channelSlug() const { return m_channelSlug; }
	int offset() const { return m_offset; }
	int limit() const { return m_limit; }
	const QString& mediaType() const { return m_mediaType; }

private:
	QString m_channelSlug;
	int     m_offset;
	int     m_limit;
	QString m_mediaType;
};

#endif

