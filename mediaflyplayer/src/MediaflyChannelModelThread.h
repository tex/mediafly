#ifndef MediaflyChannelModelThread_H
#define MediaflyChannelModelThread_H

#include "MediaflyChannelModelData.h"
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
	void entryRead(const MediaflyChannelEntry& entry);

	/* Emited when error occured during retrieving
	 * data for MediaflyChannelModel model.
	 */
	void error(const QString& errorMsg);

public slots:
	void refresh();

private slots:
	void handleEntry(const MediaflyChannelEntry& entry);

private:
	MediaflyChannelModelData m_modelData;

	void run();
};

#endif

