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
#include "nmessagebox.h"
#include "nvideoplaybacksettings.h"
#include "nssaverchannel.h"

using namespace mf;

PlayVideo::PlayVideo(QWidget *parent) :
	NBackgroundManagedWidget(parent)
{
	setupUi(this);

	setPreferredBackground(BackgroundVideoOnly);

	m_nmsControl = new NmsControl();
	if (!m_nmsControl || !m_nmsControl->Connect())
	{
		NMessageBox::warning(NULL, "Failed to connect to NMS!", "Internal error, video playback unavailable.", QMessageBox::Ok, QMessageBox::Ok, 20 * 1000);
		return;
	}
	m_nmsControl->SetMonitorEnable(false);

	m_timer = new QTimer(this);

	connect(m_timer, SIGNAL(timeout()),
	        this, SLOT(handleTimeout()));
}

void PlayVideo::quit()
{
	hide();

#ifndef NO_FUSE
	umountUrl();
#endif
	delete m_timer;

	if (m_nmsControl->Connect())
	{
		m_nmsControl->SetMonitorEnable(true);
		m_nmsControl->Disconnect();
	}
	delete m_nmsControl;
}

bool PlayVideo::show(const QModelIndex& index, QString& err)
{
	m_index = index;

	m_episodeNameLabel->setText(m_index.data(mf::EpisodeModel::titleRole).toString());
	m_showTitleLabel->setText(m_index.data(mf::EpisodeModel::showTitleRole).toString());

	QString url = m_index.data(mf::EpisodeModel::urlRole).toString();

	// Stop playing for a case we already play any video currently
	// and then unmount.

	hide();

#ifdef NO_FUSE
	return false;
#else
	umountUrl();

	// Mount httpfs filesystem with given url.

	if (mountUrl(url, err, 650) == false)
		return false;
#endif
	// Get video properties...

	NMediaInfo mediaInfo = m_nmsControl->GetMediaInfo(url);
	m_songPosition = 0;
	m_songLength = mediaInfo.GetDuration();

	// Play video...

	switch (m_nmsControl->Play(url)) {
	case 0:
		m_nmsControl->PauseUnpause();
		break;
	case 1:
		err = "Video Locked!";
		return false;
		break;
	default:
		err = "Video Format Not Supported!";
		return false;
		break;
	}

//	NSSaverClient::enable(false);

	return true;
}

void PlayVideo::hide()
{
	m_nmsControl->StopPlay();

//	NSSaverClient::enable(true);
}

void PlayVideo::play()
{
	m_nmsControl->PauseUnpause();
	m_timer->start(500);
}

void PlayVideo::pause()
{
	m_timer->stop();
	m_nmsControl->PauseUnpause();
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

void PlayVideo::seek(int sec)
{
	int seekTo = m_songPosition + sec * 1000;
	if (seekTo < 0)
		seekTo = 0;

	m_nmsControl->Seek(seekTo);
}

