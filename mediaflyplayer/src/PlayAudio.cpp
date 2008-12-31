#include "PlayAudio.h"
#include "EpisodeModel.h"
#include <QPixmap>

using namespace mf;

PlayAudio::PlayAudio(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
}

void PlayAudio::show(const QModelIndex& index)
{
	m_index = index;

	connect(m_index.model(), SIGNAL(refreshed()),
	        this, SLOT(updateImage()));

	m_iconLabel->setPixmap(m_index.data(mf::EpisodeModel::imageRole).value<QPixmap>());
	m_showTitleLabel->setText(m_index.data(mf::EpisodeModel::showTitleRole).toString());
	m_showDetailsLabel->setHtml(m_index.data(mf::EpisodeModel::descriptionRole).toString());
	m_episodeNameLabel->setText(m_index.data(mf::EpisodeModel::titleRole).toString());

	QString episodesText = tr("Episode") + " " + QString::number(m_index.row() + 1) + " " + tr("of") + " " +
		QString::number(dynamic_cast<const mf::EpisodeModel*>(m_index.model())->totalRowCount());

	m_numberOfEpisodesLabel->setText(episodesText);
}

void PlayAudio::updateImage()
{
	if (m_iconLabel->pixmap()->isNull()) {
		m_iconLabel->setPixmap(m_index.data(mf::EpisodeModel::imageRole).value<QPixmap>());
	}
}

void PlayAudio::hide()
{
}

