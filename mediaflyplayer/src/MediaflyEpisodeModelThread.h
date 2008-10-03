#ifndef MediaflyEpisodeModelThread_H
#define MediaflyEpisodeModelThread_H

#include "MediaflyEpisodeModel.h"
#include <QThread>

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
	/* Emited when MediaflyEpisodeModel model
	 * populated with new data is available.
	 */
	void refreshed(const MediaflyEpisodeModel& model);

	/* Emited when error occured during retrieving
	 * data for MediaflyEpisodeModel model.
	 */
	void error(const QString& errorMsg);

public slots:
	/* Retrieve new data for MediaflyEpisodeModel
	 * model class. Emits refreshed(...) signal when
	 * done or emits error(...) if error occured.
	 */
	void refresh(QString channelSlug, int offset, int limit, QString mediaType = "audio,video" );

private:
	void run();

	QString m_channelSlug;
	int m_offset;
	int m_limit;
	QString m_mediaType;
};

#endif

