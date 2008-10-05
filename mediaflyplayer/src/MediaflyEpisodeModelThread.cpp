#include "MediaflyEpisodeModelThread.h"
#include "Exception.h"
#include <QDebug>

MediaflyEpisodeModelThread::MediaflyEpisodeModelThread(QObject *parent) :
	QThread(parent)
{
	connect(&m_modelData, SIGNAL(entryRead(const MediaflyEpisodeEntry&)),
	        this, SLOT(handleEntry(const MediaflyEpisodeEntry&)));
	connect(&m_modelData, SIGNAL(imageRead(const QString&)),
	        this, SLOT(handleImage(const QString&)));
}

void MediaflyEpisodeModelThread::handleEntry(const MediaflyEpisodeEntry& entry)
{
	emit entryRead(entry);
}

void MediaflyEpisodeModelThread::handleImage(const QString& image)
{
	m_imagesToLoad << image;
	if (!isRunning())
		start(LowPriority);
}

void MediaflyEpisodeModelThread::run()
{
	try {
		qDebug() << "READ MENU";
		if (m_channelSlug != "") {
			m_modelData.readData(m_channelSlug, m_offset, m_limit, m_mediaType);
			m_channelSlug = "";
		}

		qDebug() << "READ IMAGES";
		for (int i = 0; i < m_imagesToLoad.size(); ++i) {
			qDebug() << "loading image:" << m_imagesToLoad[i];
			QByteArray buffer = m_modelData.readImage(m_imagesToLoad[i]);
			emit imageRead(buffer);
		}
		m_imagesToLoad.clear();
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

