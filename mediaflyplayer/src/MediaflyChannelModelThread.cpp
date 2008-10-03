#include "MediaflyChannelModelThread.h"
#include "Exception.h"

MediaflyChannelModelThread::MediaflyChannelModelThread(QObject *parent) :
	QThread(parent)
{
}

void MediaflyChannelModelThread::run()
{
	try {
		MediaflyChannelModel model;
		model.refresh();
		emit refreshed(model);
	}
	catch (Exception &e) {
		emit error(e.getError());
	}
}

void MediaflyChannelModelThread::refresh()
{
	start(LowPriority);
}

