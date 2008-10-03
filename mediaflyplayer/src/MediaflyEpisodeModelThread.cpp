#include "MediaflyEpisodeModelThread.h"
#include "Exception.h"

MediaflyEpisodeModelThread::MediaflyEpisodeModelThread(QObject *parent) :
	QThread(parent)
{
}

void MediaflyEpisodeModelThread::run()
{
	try {
		MediaflyEpisodeModel model;
		model.refresh(m_channelSlug, m_offset, m_limit, m_mediaType);
		emit refreshed(model);
	}
	catch (Exception &e) {
		emit error(e.getError());
	}
}

void MediaflyEpisodeModelThread::refresh(QString channelSlug, int offset, int limit, QString mediaType)
{
	m_channelSlug = channelSlug;
	m_offset = offset;
	m_limit = limit;
	m_mediaType = mediaType;

	start(LowPriority);
}

