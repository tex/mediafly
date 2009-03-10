/*
 *  Copyright(C) 2009 Milan Svoboda. 
 *               <milan.svoboda@centrum.cz>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *  
 *
 *  This program is distributed in the hope that, in addition to its 
 *  original purpose to support Neuros hardware, it will be useful 
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************/

#include "PlayAudioPhonon.h"
#include "EpisodeModel.h"
#include <QKeyEvent>
#include <QScrollBar>
#include <QTime>
#include <QPixmap>
#include <QVariant>

using namespace mf;

PlayAudio::PlayAudio(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	m_mediaObject = new Phonon::MediaObject(this);
	m_audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	Phonon::createPath(m_mediaObject, m_audioOutput);

	connect(m_mediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
		this, SLOT(handleStateChanged(Phonon::State, Phonon::State)));
	connect(m_mediaObject, SIGNAL(finished()),
	        this, SLOT(handleFinished()));
	connect(m_mediaObject, SIGNAL(tick(qint64)),
	        this, SLOT(handleTick(qint64)));
}

void PlayAudio::handleStateChanged(Phonon::State newState, Phonon::State oldState)
{
	emit stateChange();
}

void PlayAudio::handleFinished()
{
	emit stateChange();
}

void PlayAudio::handleTick(qint64)
{
	emit stateChange();
}

void PlayAudio::quit()
{
	hide();
}

bool PlayAudio::show(const QModelIndex& index, QString& err)
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

	// Start playing a new song.

	QString url = m_index.data(mf::EpisodeModel::urlRole).toString();
	m_mediaObject->setCurrentSource(url);

	return true;
}

void PlayAudio::play()
{
	m_mediaObject->play();
	m_mediaObject->setTickInterval(1000);
}

void PlayAudio::pause()
{
	m_mediaObject->pause();
}

void PlayAudio::updateImage()
{
	if (m_iconLabel->pixmap()->isNull()) {
		m_iconLabel->setPixmap(m_index.data(mf::EpisodeModel::imageRole).value<QPixmap>());
	}
}

void PlayAudio::hide()
{
	m_mediaObject->setTickInterval(0);
	m_mediaObject->stop();
}

void PlayAudio::getState(int& songPosition, int& songLength, bool& isFinished)
{
	songPosition = m_mediaObject->currentTime();
	songLength = m_mediaObject->totalTime();

	switch (m_mediaObject->state()) {
	case Phonon::StoppedState:
	case Phonon::PausedState:
	case Phonon::ErrorState:
		isFinished = true;
		break;
	default:
		isFinished = false;
		break;
	}
}

void PlayAudio::seek(int sec)
{
	m_mediaObject->seek(sec * 1000);
}

void PlayAudio::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_PageUp:
		if (QScrollBar* bar = m_showDetailsLabel->verticalScrollBar())
			bar->triggerAction(QAbstractSlider::SliderPageStepSub);
		break;
	case Qt::Key_PageDown:
		if (QScrollBar* bar = m_showDetailsLabel->verticalScrollBar())
			bar->triggerAction(QAbstractSlider::SliderPageStepAdd);
		break;
	}
}

