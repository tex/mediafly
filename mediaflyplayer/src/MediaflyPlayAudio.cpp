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

	m_iconLabel->setPixmap(m_index.data(MediaflyEpisodeModel::imageRole).value<QPixmap>());
	m_showTitleLabel->setText(m_index.data(MediaflyEpisodeModel::showTitleRole).toString());
	m_showDetailsLabel->setHtml(m_index.data(MediaflyEpisodeModel::descriptionRole).toString());
	m_episodeNameLabel->setText(m_index.data(MediaflyEpisodeModel::titleRole).toString());

	QString episodesText = tr("Episode") + " " + QString::number(m_index.row() + 1) + " " + tr("of") + " " +
		QString::number(dynamic_cast<const MediaflyEpisodeModel*>(m_index.model())->totalRowCount());

	m_numberOfEpisodesLabel->setText(episodesText);
}

void MediaflyPlayAudio::updateImage()
{
	if (m_iconLabel->pixmap()->isNull()) {
		m_iconLabel->setPixmap(m_index.data(MediaflyEpisodeModel::imageRole).value<QPixmap>());
	}
}

void MediaflyPlayAudio::hide()
{
}

