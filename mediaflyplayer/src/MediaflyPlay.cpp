#include "MediaflyPlay.h"
#include "MediaflyEpisodeModel.h"

MediaflyPlay::MediaflyPlay(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
}

void MediaflyPlay::show(const QModelIndex& index)
{
	m_index = index;

	if (m_index.data(MediaflyEpisodeModel::formatRole).toString().compare("Video", Qt::CaseInsensitive) == 0)
	{
		video->show(m_index);
		stackedWidget->setCurrentWidget(video);
	}
	else
	{
		audio->show(m_index);
		stackedWidget->setCurrentWidget(audio);
	}

//	connect(m_index.model(), SIGNAL(refreshed()),
//	        this, SLOT(updateImage()));

	// TODO: Decide what to show based on audio/video content
}

//void MediaflyPlay::updateImage()
//{
/*
	if (m_icon->pixmap()->isNull()) {
		m_icon->setPixmap(m_index.data(MediaflyEpisodeModel::imageRole).value<QPixmap>());
	}
*/
//}

void MediaflyPlay::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Escape:
		video->hide();
		audio->hide();
		emit back();
		break;
	default:
		event->ignore();
		break;
	}
}

