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

using namespace mf;

Play::Play(QWidget *parent) :
	NBackgroundManagedWidget(parent),
	m_state(MP_PAUSE),
	m_output(NULL)
{
	setupUi(this);

	setPreferredBackground(BackgroundVideoOnly);

	regMediaKey();

	connect(m_audio, SIGNAL(stateChange()),
	        this, SLOT(handleStateChange()));
	connect(m_video, SIGNAL(stateChange()),
	        this, SLOT(handleStateChange()));
}

Play::~Play()
{
	unregMediaKey();
}

void Play::regMediaKey()
{
        m_mediakeyChannel.regExclusiveMediakey(this, Qt::Key_MediaPlay);
        m_mediakeyChannel.regExclusiveMediakey(this, Qt::Key_MediaStop);
        m_mediakeyChannel.regExclusiveMediakey(this, Qt::Key_MediaNext);
        m_mediakeyChannel.regExclusiveMediakey(this, Qt::Key_MediaPrevious);
        m_mediakeyChannel.regExclusiveMediakey(this, Qt::Key_Back);
}

void Play::unregMediaKey()
{
        m_mediakeyChannel.unregExclusiveMediakey(this, Qt::Key_MediaPlay);
        m_mediakeyChannel.unregExclusiveMediakey(this, Qt::Key_MediaStop);
        m_mediakeyChannel.unregExclusiveMediakey(this, Qt::Key_MediaNext);
        m_mediakeyChannel.unregExclusiveMediakey(this, Qt::Key_MediaPrevious);
        m_mediakeyChannel.unregExclusiveMediakey(this, Qt::Key_Back);
}

void Play::handleStateChange()
{
	// Ignore state change if our state is different that MP_PLAY.

	if (m_state != MP_PLAY)
		return;

	QString slug = m_index.data(mf::EpisodeModel::slugRole).toString();

	// Get current position and length of the episode. getState returns
	// time values in miliseconds.

	int position, length;
	m_output->getState(position, length);

	// Experience_PostExperienceForEpisode expects time values in seconds.

	position /= 1000;
	length /= 1000;

	m_progressBar->setRange(0, length);
	m_progressBar->setValue(position);

	Mediafly::getMediafly()->Experience_PostExperienceForEpisode(&m_checkResponseOk, slug, position, length);

	emit stateChange();
}

void Play::handleChannelsButtonClicked()
{
	m_video->hide();
	m_audio->hide();
	emit backToChannelsMenu();
}

void Play::handleNextEpisodeButtonClicked()
{
	if (mf::EpisodeModel::advanceToNextEpisode(m_index))
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
	default:
		Q_ASSERT(false);
	}
	updateStateIndicator(m_state);
}

void Play::updateStateIndicator(enum State state)
{
	switch (state) {
	case MP_PAUSE:
	{
		m_playStateButton->setText("|");
		m_output->pause();
		break;
	}
	case MP_PLAY:
	{
		m_playStateButton->setText(">");
		m_output->play();
		break;
	}
	default:
		Q_ASSERT(false);
	}
}

void Play::update()
{
	if (m_output == m_video)
	{
		qDebug() << "Recognized as video";

		m_audio->hide();
		m_video->show(m_index);
		m_stackedWidget->setCurrentWidget(m_video);
	}
	else
	{
		qDebug() << "Recognized as audio";

		m_audio->show(m_index);
		m_video->hide();
		m_stackedWidget->setCurrentWidget(m_audio);
	}

	m_state = MP_PLAY;
	updateStateIndicator(m_state);

	emit stateChange();
}

void Play::show(const QModelIndex& index)
{
	m_index = index;

	bool isVideo = m_index.data(mf::EpisodeModel::formatRole)
	                      .toString()
	                      .startsWith("Video", Qt::CaseInsensitive);

	if (isVideo)
		m_output = m_video;
	else
		m_output = m_audio;

	update();
}

void Play::handleEscape()
{
	m_video->hide();
	m_audio->hide();
	emit back();
}

void Play::handleMediaStop()
{
	switch (m_state) {
	case MP_PLAY:
		m_state = MP_PAUSE;
		break;
	default:
		return;
	}
	updateStateIndicator(m_state);
}

void Play::handleMediaNext()
{
	handleNextEpisodeButtonClicked();
}

void Play::handleBack()
{
	handleChannelsButtonClicked();
}

void Play::keyPressEvent(QKeyEvent *event)
{
	qDebug() << __PRETTY_FUNCTION__ << "event->key() ==" << event->key();

	switch (event->key()) {
	case Qt::Key_Escape:
		handleEscape();
		break;
	case Qt::Key_MediaPlay:
		handlePlayStateButtonClicked();
		break;
	case Qt::Key_MediaStop:
		handleMediaStop();
		break;
	case Qt::Key_MediaNext:
		handleMediaNext();
		break;
	case Qt::Key_Up:
		emit showPlayqueue();
		break;
	case Qt::Key_Back:
		handleBack();
		break;
	default:
		event->ignore();
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
	int position, length;

	currentIndex = m_index;
	m_output->getState(position, length);
	songPosition = toTime(position);
	songLength = toTime(length);
}

