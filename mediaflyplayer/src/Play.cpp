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

#include "Play.h"
#include "EpisodeModel.h"
#include <QTime>
#include "nhelpbox.h"

using namespace mf;

Play::Play(QWidget *parent) :
	NBackgroundManagedWidget(parent),
	m_state(MP_PAUSE),
	m_output(m_audio),
	m_ready(false)
{
	setupUi(this);

	setPreferredBackground(BackgroundVideoOnly);

	connect(m_audio, SIGNAL(stateChange()),
	        this, SLOT(handleStateChange()));
	connect(m_video, SIGNAL(stateChange()),
	        this, SLOT(handleStateChange()));
}

void Play::quit()
{
	m_video->quit();
	m_audio->quit();
}

void Play::handleStateChange()
{
	// Ignore state change if our state is different that MP_PLAY.

	if (m_state != MP_PLAY)
		return;

	// Get current position and length of the episode. getState returns
	// time values in miliseconds.

	int position;
	int length;
	bool isFinished;

	// Get current position and length of the episode. Both are
	// in miliseconds.

	m_output->getState(position, length, isFinished);

	// Experience_PostExperienceForEpisode expects time values in seconds.
	// NTimerBar expect time values in seconds.

	position /= 1000;
	length /= 1000;

	QString slug = m_index.data(mf::EpisodeModel::slugRole).toString();
	if (!slug.isEmpty())
	{
		Mediafly::getMediafly()->Experience_PostExperienceForEpisode(&m_checkResponseOk, slug, position, length);
	}

	// If lenght of the episode is unknown, set the 'default' to
	// 99 hours, 99 minutes and 99 seconds.

	if (length == 0)
		length = 99 * 60 * 60 + 99 * 60 + 99;

	m_progressBar->setMaximum(length);
	m_progressBar->setValue(position);
	m_progressBar->update();

	if (isFinished && m_ready)
	{
		// When one episode finishes advance to
		// next one.

		handleNextEpisodeButtonClicked();
	}

	emit stateChange();
}

void Play::handleNextEpisodeButtonClicked()
{
	if (mf::EpisodeModel::advanceToNextEpisode(m_index))
	{
		update();
	}
}

void Play::handlePreviousEpisodeButtonClicked()
{
	if (mf::EpisodeModel::advanceToPreviousEpisode(m_index))
	{
		update();
	}
}

void Play::handlePlayStateButtonClicked()
{
	switch (m_state) {
	case MP_PAUSE:
		m_state = MP_PLAY;
		break;
	case MP_PLAY:
		m_state = MP_PAUSE;
		break;
	case MP_ERROR:
		update();
		break;
	}
	updateState();
}

void Play::updateState()
{
	switch (m_state) {
	case MP_PAUSE:
		m_playStateLabel->setText(">");
		m_output->pause();
		break;
	case MP_PLAY:
		m_playStateLabel->setText("||");
		m_output->play();
		break;
	case MP_ERROR:
		m_playStateLabel->setText("X");
		break;
	}
}

void Play::show(const QModelIndex& index)
{
	m_index = index;
	m_ready = true;
	update();
}

void Play::update()
{
	bool isVideo = m_index.data(mf::EpisodeModel::formatRole)
	                      .toString()
	                      .startsWith("Video", Qt::CaseInsensitive);

	m_statusLabel->setText(tr("Loading..."));

	QString err;

	if (isVideo)
	{
		if (m_output == m_audio)
			m_audio->hide();
		m_output = m_video;

		m_stackedWidget->setCurrentWidget(m_video);
	}
	else
	{
		if (m_output == m_video)
			m_video->hide();
		m_output = m_audio;

		m_stackedWidget->setCurrentWidget(m_audio);
	}

	bool ret = m_output->show(m_index, err);
	if (ret == true)
	{
		m_statusLabel->clear();
		m_state = MP_PLAY;
		emit stateChange();
	}
	else
	{
		m_statusLabel->setText(err);
		m_state = MP_ERROR;
	}
	updateState();
}

void Play::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_PageUp:
	case Qt::Key_PageDown:
		if (m_output == m_audio)
			m_audio->keyPressEvent(event);
		break;
	case Qt::Key_Left:
		m_ready = false;
		m_video->hide();
		m_audio->hide();
		emit back();
		break;
	case Qt::Key_Escape:
		m_ready = false;
		m_video->hide();
		m_audio->hide();
		emit backToChannelMenu();
		break;
	case Qt::Key_Up:
		emit showPlayqueue();
		break;
	case Qt::Key_MediaPlay:
		handlePlayStateButtonClicked();
		break;
	case Qt::Key_MediaNext:
		handleNextEpisodeButtonClicked();
		break;
	case Qt::Key_MediaPrevious:
		handlePreviousEpisodeButtonClicked();
		break;
	case Qt::Key_Back:
		m_output->seek(-30);
		break;
	case Qt::Key_Forward:
		m_output->seek(+30);
		break;
	case Qt::Key_Help:
		NHelpBox::NHelpBoxNew(tr("Possible keys"),
		                      tr("Left - Back to episode list\n") +
		                      tr("Back - Back to channel list\n") +
		                      tr("Up - Show play queue\n") +
		                      tr("Play/Pause - Play/Pause media playback\n") +
		                      tr("Next - Play next episode\n") +
		                      tr("Previous - Play previous episode\n") +
		                      tr("Back/Forward - Seek 30 seconds back/forward\n"));
		break;
	}
}

/**
 * Converts seconds to nice string.
 * We assume that no playcast will be longer than 24 hours...
 */
QString Play::toTime(unsigned int msec) const
{
	unsigned int sec = msec / 1000;
	int h = sec / (60 * 60);
	int m = (sec / 60) - (h * 60);
	int s = sec - (h * 60 * 60) - (m * 60);

	QTime time(h, m, s);
	return time.toString();
}

void Play::getState(QModelIndex &currentIndex, QString &songPosition, QString &songLength)
{
	int position;
	int length;
	bool isFinished;

	currentIndex = m_index;
	m_output->getState(position, length, isFinished);
	songPosition = toTime(position);
	songLength = toTime(length);
}

