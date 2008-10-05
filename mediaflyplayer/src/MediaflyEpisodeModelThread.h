#ifndef MediaflyEpisodeModelThread_H
#define MediaflyEpisodeModelThread_H

#include "MediaflyEpisodeModelData.h"
#include <QThread>
#include <QStringList>

class MediaflyEpisodeModel;

/*
 * This class retrieves data that use to populate
 * MediaflyEpisodeModel model class.
 */
class MediaflyEpisodeModelThread : public QThread
{
	Q_OBJECT
public:
	MediaflyEpisodeModelThread(QObject *parent = 0);
	~MediaflyEpisodeModelThread() { }

signals:
	void entryRead(const MediaflyEpisodeEntry& entry);
	void imageRead(const QByteArray& buffer);

	/* Emited when error occured during retrieving
	 * data for MediaflyEpisodeModel model.
	 */
	void error(const QString& errorMsg);

public slots:
	void refresh(QString channelSlug, int offset, int limit, QString mediaType = "audio,video");

private slots:
	void handleEntry(const MediaflyEpisodeEntry& entry);
	void handleImage(const QString& imageUrl);

private:
	MediaflyEpisodeModelData m_modelData;
	
	QString m_channelSlug;
	int     m_offset;
	int     m_limit;
	QString m_mediaType;

	QStringList m_imagesToLoad;

	void run();
};

#endif

