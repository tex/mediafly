#include "PlayVideo.h"
#include "EpisodeModel.h"

using namespace mf;

PlayVideo::PlayVideo(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
}

void PlayVideo::show(const QModelIndex& index)
{
	m_index = index;

	m_episodeNameLabel->setText(m_index.data(mf::EpisodeModel::titleRole).toString());
	m_showTitleLabel->setText(m_index.data(mf::EpisodeModel::showTitleRole).toString());
	
	// TODO PLAY
}

void PlayVideo::hide()
{
	// TODO STOP
}

