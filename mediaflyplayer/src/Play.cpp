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
	QWidget(parent),
	m_state(PAUSE)
{
	setupUi(this);

	connect(m_changeChannelsButton, SIGNAL(clicked()),
	        this, SLOT(handleChannelsButtonClicked()));
	connect(m_nextEpisodeButton, SIGNAL(clicked()),
	        this, SLOT(handleNextEpisodeButtonClicked()));
	connect(m_playqueueButton, SIGNAL(clicked()),
	        this, SLOT(handlePlayqueueButtonClicked()));
	connect(m_playStateButton, SIGNAL(clicked()),
	        this, SLOT(handlePlayStateButtonClicked()));
	connect(m_audio, SIGNAL(stateChange()),
	        this, SLOT(handleStateChange()));
	connect(m_video, SIGNAL(stateChange()),
	        this, SLOT(handleStateChange()));
}

void Play::handleStateChange()
{
	int position, length;

	QString slug = m_index.data(mf::EpisodeModel::slugRole).toString();
	QString format = m_index.data(mf::EpisodeModel::formatRole).toString();

	// Get current position and length of the episode. getState returns
	// time values in miliseconds.

	if (format.startsWith("Video", Qt::CaseInsensitive) == true)
		m_video->getState(position, length);
	else
		m_audio->getState(position, length);

	// Experience_PostExperienceForEpisode expects time values in seconds.

	position /= 1000;
	length /= 1000;

	if (length == 0)
	{
		// Total length of the song is unknown. Set it to
		// song position + 1 to let the user know that
		// total length is unknown (it looks good on the screen).

		length = position + 1;
	}
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

void Play::handlePlayqueueButtonClicked()
{
	emit showPlayqueue();
}

void Play::handlePlayStateButtonClicked()
{
	switch (m_state) {
	case PAUSE:
		m_state = PLAY;
		break;
	case PLAY:
		m_state = PAUSE;
		break;
	default:
		Q_ASSERT(false);
	}
	updateStateIndicator(m_state);
}

void Play::updateStateIndicator(enum State state)
{
	switch (state) {
	case PAUSE:
	{
		m_playStateButton->setText("|");
		QString format = m_index.data(mf::EpisodeModel::formatRole).toString();
		if (format.startsWith("Video", Qt::CaseInsensitive) == true)
			m_video->pause();
		else
			m_audio->pause();
		break;
	}
	case PLAY:
	{
		m_playStateButton->setText(">");
		QString format = m_index.data(mf::EpisodeModel::formatRole).toString();
		if (format.startsWith("Video", Qt::CaseInsensitive) == true)
			m_video->play();
		else
			m_audio->play();
		break;
	}
	default:
		Q_ASSERT(false);
	}
}

void Play::update()
{
	QString format = m_index.data(mf::EpisodeModel::formatRole).toString();
	if (format.startsWith("Video", Qt::CaseInsensitive) == true)
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

	m_state = PLAY;
	updateStateIndicator(m_state);

	emit stateChange();
}

void Play::show(const QModelIndex& index)
{
	m_index = index;

	update();
}

void Play::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Escape:
		m_video->hide();
		m_audio->hide();
		emit back();
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
	currentIndex = m_index;

	int position, length;

	QString format = m_index.data(mf::EpisodeModel::formatRole).toString();
	if (format.startsWith("Video", Qt::CaseInsensitive) == true)
	{
		m_video->getState(position, length);
	}
	else
	{
		m_audio->getState(position, length);
	}

	songPosition = toTime(position);
	songLength = toTime(length);
}

