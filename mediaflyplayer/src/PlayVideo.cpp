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

	setPreferredBackground(NBackgroundManagedWidget::BackgroundVideoOnly);

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

PlayVideo::~PlayVideo()
{
	hide();

	m_timer->stop();
	delete m_timer;

	m_nmsControl->SetMonitorEnable(true);
	m_nmsControl->Disconnect();
	delete m_nmsControl;
}

void PlayVideo::show(const QModelIndex& index)
{
	m_index = index;

	m_episodeNameLabel->setText(m_index.data(mf::EpisodeModel::titleRole).toString());
	m_showTitleLabel->setText(m_index.data(mf::EpisodeModel::showTitleRole).toString());
	
	setUrl(m_index.data(mf::EpisodeModel::urlRole).toString());

	NSSaverClient::enable(false);
}

QString PlayVideo::mountUrl(QString url)
{
#if 0
	QString cmd;

	cmd = "mkdir /media/SD-card/httpfs_mp";
	system(cmd.toAscii());

	cmd = "/media/SD-card/httpfs " + url + " /media/SD-card/httpfs_mp";
	system(cmd.toAscii());

	QString ret = "/tmp/httpfs" + url.right(url.size() - url.lastIndexOf("/"));
#endif
	QString ret = "/media/SD-card/title2.avi";
	m_episodeNameLabel->setText(ret);
	m_showTitleLabel->setText("---");

	return ret;
}

void PlayVideo::umountUrl()
{
#if 0
	QString cmd;

	cmd = "fusermount -u /tmp/httpfs";
	system(cmd.toAscii());
#endif
}

void PlayVideo::setUrl(QString url)
{
	// Mount httpfs filesystem with given url.

	url = mountUrl(url);

	// Get video properties...

	m_mediaInfo = m_nmsControl->GetMediaInfo(url);
	m_songLength = m_mediaInfo.GetDuration();

	qDebug() << __PRETTY_FUNCTION__ << "url:" << url << ", song length:" << m_songLength;

	// Play video...

	switch (m_nmsControl->Play(url)) {
	case 0:
		m_timer->start(500);
		m_showTitleLabel->setText("OK");
		break;
	case 1:
		m_showTitleLabel->setText("video locked");
		break;
	default:
		m_showTitleLabel->setText("video format not known");
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

void PlayVideo::hide()
{
	qDebug() << __PRETTY_FUNCTION__;

	umountUrl();

	m_timer->stop();
	m_nmsControl->StopPlay();

	NSSaverClient::enable(true);
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

