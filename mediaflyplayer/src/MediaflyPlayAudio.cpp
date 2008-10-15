#include "MediaflyPlayAudio.h"
#include "MediaflyEpisodeModel.h"
#include <QPixmap>

MediaflyPlayAudio::MediaflyPlayAudio(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
}

void MediaflyPlayAudio::show(const QModelIndex& index)
{
	m_index = index;

	connect(m_index.model(), SIGNAL(refreshed()),
	        this, SLOT(updateImage()));

	iconLabel->setPixmap(m_index.data(MediaflyEpisodeModel::imageRole).value<QPixmap>());
	showTitleLabel->setText(m_index.data(MediaflyEpisodeModel::showTitleRole).toString());
	showDetailsLabel->setHtml(m_index.data(MediaflyEpisodeModel::descriptionRole).toString());
	episodeNameLabel->setText(m_index.data(MediaflyEpisodeModel::titleRole).toString());
	numberOfEpisodesLabel->setText(m_index.row() + " / " + dynamic_cast<const MediaflyEpisodeModel *>(m_index.model())->totalRowCount());

	// TODO PLAY
}

void MediaflyPlayAudio::updateImage()
{
	if (iconLabel->pixmap()->isNull()) {
		iconLabel->setPixmap(m_index.data(MediaflyEpisodeModel::imageRole).value<QPixmap>());
	}
}

void MediaflyPlayAudio::hide()
{
	// TODO STOP PLAY?
}

