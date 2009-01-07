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

extern QString currentPath;

const QString PlayVideo::m_mountPoint = "/tmp/httpfs_mf";

PlayVideo::PlayVideo(QWidget *parent) :
	NBackgroundManagedWidget(parent)
{
	setupUi(this);

	setPreferredBackground(BackgroundVideoOnly);

	m_nmsControl = new NmsControl();
	if (!m_nmsControl || !m_nmsControl->Connect())
	{
		NMessageBox::warning(NULL, "Failed to connect to NMS!", "Internal error, video playback unavailable.", QMessageBox::Ok, QMessageBox::Ok, 20 * 1000);
	}
	m_nmsControl->SetMonitorEnable(false);

	m_timer = new QTimer(this);

	connect(m_timer, SIGNAL(timeout()),
	        this, SLOT(handleTimeout()));
}

void PlayVideo::quit()
{
	hide();

	delete m_timer;

	if (m_nmsControl->Connect())
	{
		m_nmsControl->SetMonitorEnable(true);
		m_nmsControl->Disconnect();
	}
	delete m_nmsControl;
}

void PlayVideo::show(const QModelIndex& index)
{
	m_index = index;

	m_episodeNameLabel->setText(m_index.data(mf::EpisodeModel::titleRole).toString());
	m_showTitleLabel->setText(m_index.data(mf::EpisodeModel::showTitleRole).toString());
	m_statusLabel->setText(tr("Loading..."));

	setUrl(m_index.data(mf::EpisodeModel::urlRole).toString());

	NSSaverClient::enable(false);
}

void PlayVideo::hide()
{
	qDebug() << __PRETTY_FUNCTION__;

	m_nmsControl->StopPlay();
	m_timer->stop();

	umountUrl();

	NSSaverClient::enable(true);
}

bool PlayVideo::mountUrl(QString& url)
{
	int r;
	QString cmd;

	cmd = "mkdir " + m_mountPoint;
	system(cmd.toAscii());

	cmd = currentPath + "/httpfs \"" + url + "\" " + m_mountPoint;
	r = system(cmd.toAscii());
	if (r == -1)
	{
		m_statusLabel->setText(tr("Insuficient recources!"));
		return false;
	}
	else if (WEXITSTATUS(r) != 0)
	{
		m_statusLabel->setText(tr("Failed to load video!"));
		return false;
	}

	url = m_mountPoint + url.right(url.size() - url.lastIndexOf("/"));

	return true;
}

void PlayVideo::umountUrl()
{
	QString cmd;

	cmd = "fusermount -u " + m_mountPoint;
	system(cmd.toAscii());
}

void PlayVideo::setUrl(QString url)
{
	// Try to unmount our mount point for a case it was mounted...

	umountUrl();

	// Mount httpfs filesystem with given url.

	if (mountUrl(url) == false)
		return;

	// Get video properties...

	m_mediaInfo = m_nmsControl->GetMediaInfo(url);
	m_songLength = m_mediaInfo.GetDuration();

	qDebug() << __PRETTY_FUNCTION__ << "url:" << url << ", song length:" << m_songLength;

	// Play video...

	switch (m_nmsControl->Play(url)) {
	case 0:
		m_timer->start(500);
		m_statusLabel->setText("");
		break;
	case 1:
		m_statusLabel->setText("Video Locked!");
		break;
	default:
		m_statusLabel->setText("Video Format Not Supported!");
		break;
	}
}

void PlayVideo::play()
{
	qDebug() << __PRETTY_FUNCTION__;

	m_nmsControl->PauseUnpause();
}

void PlayVideo::pause()
{
	qDebug() << __PRETTY_FUNCTION__;

	m_nmsControl->PauseUnpause();
}

void PlayVideo::handleTimeout()
{
	m_songPosition = m_nmsControl->GetPlayTime();

	qDebug() << __PRETTY_FUNCTION__ << "song position:" << m_songPosition;

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

