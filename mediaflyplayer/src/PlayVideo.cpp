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

#include "PlayVideo.h"
#include "EpisodeModel.h"

using namespace mf;

PlayVideo::PlayVideo(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	m_nmsControl = new NmsControl();
	m_nmsControl->Connect();

	m_timer = new QTimer(this);
	m_timer->setInterval(500);

	connect(m_timer, SIGNAL(timeout()),
	        this, SLOT(handleTimeout()));
}

PlayVideo::~PlayVideo()
{
	hide();

	m_timer->stop();
	delete m_timer;

	m_nmsControl->Disconnect();
}

void PlayVideo::show(const QModelIndex& index)
{
	m_index = index;

	m_episodeNameLabel->setText(m_index.data(mf::EpisodeModel::titleRole).toString());
	m_showTitleLabel->setText(m_index.data(mf::EpisodeModel::showTitleRole).toString());
	
	setUrl(m_index.data(mf::EpisodeModel::urlRole).toString());
}

void PlayVideo::setUrl(QString url)
{
	// Get video properties...

	m_mediaInfo = m_nmsControl->GetMediaInfo(url);
	m_songLength = m_mediaInfo.GetDuration();

	// Play video...

	m_nmsControl->Play(url);
	m_timer->start();
}

void PlayVideo::play()
{
	m_nmsControl->PauseUnpause();
}

void PlayVideo::pause()
{
	m_nmsControl->PauseUnpause();
}

void PlayVideo::hide()
{
	m_timer->stop();
	m_nmsControl->StopPlay();
}

void PlayVideo::handleTimeout()
{
	m_songPosition = m_nmsControl->GetPlayTime();
	emit stateChange();
}

void PlayVideo::getState(int& songPosition, int& songLength)
{
	songPosition = m_songPosition;
	songLength = m_songLength;
}

