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

	QString episodesText = "Episode " + QString::number(m_index.row() + 1) + " of " +
		QString::number(dynamic_cast<const MediaflyEpisodeModel*>(m_index.model())->totalRowCount());

	numberOfEpisodesLabel->setText(episodesText);

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

