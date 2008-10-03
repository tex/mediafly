#ifndef MediaflyChannelModelThread_H
#define MediaflyChannelModelThread_H

#include <QThread>

class MediaflyChannelModel;

/*
 * This class retrieves data that use to populate
 * MediaflyChannelModel model class.
 */
class MediaflyChannelModelThread : public QThread
{
	Q_OBJECT
public:
	MediaflyChannelModelThread(QObject *parent = 0);
	~MediaflyChannelModelThread() { }

signals:
	/* Emited when MediaflyChannelModel model
	 * populated with new data is available.
	 */
	void refreshed(const MediaflyChannelModel& model);

	/* Emited when error occured during retrieving
	 * data for MediaflyChannelModel model.
	 */
	void error(const QString& errorMsg);

public slots:
	/* Retrieve new data for MediaflyChannelModel
	 * model class. Emits refreshed(...) signal when
	 * done or emits error(...) if error occured.
	 */
	void refresh();

private:
	void run();
};

#endif

