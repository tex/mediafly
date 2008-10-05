#include "MediaflyChannelModelThread.h"
#include "Exception.h"

MediaflyChannelModelThread::MediaflyChannelModelThread(QObject *parent) :
	QThread(parent)
{
	connect(&m_modelData, SIGNAL(entryRead(const MediaflyChannelEntry&)),
	        this, SLOT(handleEntry(const MediaflyChannelEntry&)));
}

void MediaflyChannelModelThread::handleEntry(const MediaflyChannelEntry& entry)
{
	emit entryRead(entry);
}

void MediaflyChannelModelThread::run()
{
	try {
		m_modelData.readData();
	}
	catch (Exception &e) {
		emit error(e.getError());
	}
}

void MediaflyChannelModelThread::refresh()
{
	start(LowPriority);
}

