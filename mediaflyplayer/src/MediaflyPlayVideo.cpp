#include "MediaflyPlayVideo.h"
#include "MediaflyEpisodeModel.h"

MediaflyPlayVideo::MediaflyPlayVideo(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
}

void MediaflyPlayVideo::show(const QModelIndex& index)
{
	m_index = index;

	m_episodeNameLabel->setText(m_index.data(MediaflyEpisodeModel::titleRole).toString());
	m_showTitleLabel->setText(m_index.data(MediaflyEpisodeModel::showTitleRole).toString());
	
	// TODO PLAY
}

void MediaflyPlayVideo::hide()
{
	// TODO STOP
}

