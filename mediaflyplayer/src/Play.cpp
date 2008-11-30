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

using namespace mf;

Play::Play(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(m_changeChannelsButton, SIGNAL(clicked()),
	        this, SLOT(handleChannelsButtonClicked()));
	connect(m_nextEpisodeButton, SIGNAL(clicked()),
	        this, SLOT(handleNextEpisodeButtonClicked()));
	connect(m_playqueueButton, SIGNAL(clicked()),
	        this, SLOT(handlePlayqueueButtonClicked()));
	connect(m_audio, SIGNAL(stateChange()),
	        this, SLOT(handleStateChange()));
	connect(m_video, SIGNAL(stateChange()),
	        this, SLOT(handleStateChange()));
}

void Play::handleStateChange()
{
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

void Play::updateStateIndicator(enum State state)
{
	switch (state) {
	case PAUSE:
	{
		m_playStateButton->setText("||");
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

void Play::getState(QModelIndex &currentIndex, QString &songPosition, QString &songLength)
{
	currentIndex = m_index;

	QString format = m_index.data(mf::EpisodeModel::formatRole).toString();
	if (format.startsWith("Video", Qt::CaseInsensitive) == true)
	{
		m_video->getState(songPosition, songLength);
	}
	else
	{
		m_audio->getState(songPosition, songLength);
	}
}

