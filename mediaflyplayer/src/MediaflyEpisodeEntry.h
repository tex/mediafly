#ifndef MediaflyEpisodeEntry_H
#define MediaflyEpisodeEntry_H

#include <QString>
#include <QMetaType>

class MediaflyEpisodeEntry
{
public:
	MediaflyEpisodeEntry()
	{
		qRegisterMetaType<MediaflyEpisodeEntry>("MediaflyEpisodeEntry");
	}
	MediaflyEpisodeEntry(const MediaflyEpisodeEntry& obj) :
		m_title (obj.m_title),
		m_slug (obj.m_slug),
		m_description (obj.m_description),
		m_format (obj.m_format),
		m_url (obj.m_url),
		m_urlOriginal (obj.m_urlOriginal),
		m_published (obj.m_published),
		m_showSlug (obj.m_showSlug),
		m_showTitle (obj.m_showTitle),
		m_imageUrl (obj.m_imageUrl),
		m_channel (obj.m_channel)
	{
		qRegisterMetaType<MediaflyEpisodeEntry>("MediaflyEpisodeEntry");
	}

	MediaflyEpisodeEntry(QString title, QString slug, QString description, QString format,
	                     QString url, QString urlOriginal, QString published, QString showSlug,
	                     QString showTitle, QString imageUrl, QString channel) :
		m_title (title),
		m_slug (slug),
		m_description (description),
		m_format (format),
		m_url (url),
		m_urlOriginal (urlOriginal),
		m_published (published),
		m_showSlug (showSlug),
		m_showTitle (showTitle),
		m_imageUrl (imageUrl),
		m_channel (channel)
	{
		qRegisterMetaType<MediaflyEpisodeEntry>("MediaflyEpisodeEntry");
	}

	const QString& title() const { return m_title; }
	const QString& slug() const { return m_slug; }
	const QString& description() const { return m_description; }
	const QString& format() const { return m_format; }
	const QString& url() const { return m_url; }
	const QString& urlOriginal() const { return m_urlOriginal; }
	const QString& published() const { return m_published; }
	const QString& showSlug() const { return m_showSlug; }
	const QString& showTitle() const { return m_showTitle; }
	const QString& imageUrl() const { return m_imageUrl; }
	const QString& channel() const { return m_channel; }

private:
	QString m_title;
	QString m_slug;
	QString m_description;
	QString m_format;
	QString m_url;
	QString m_urlOriginal;
	QString m_published;
	QString m_showSlug;
	QString m_showTitle;
	QString m_imageUrl;
	QString m_channel;
};

Q_DECLARE_METATYPE(MediaflyEpisodeEntry);

#endif

