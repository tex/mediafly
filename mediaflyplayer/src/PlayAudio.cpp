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

#include "PlayAudio.h"
#include "EpisodeModel.h"
#include "nxmmsmainloop.h"
#include "nmessagebox.h"

#include <QTime>
#include <QPixmap>
#include <QVariant>

#define XMMS2_CONNECTION_PATH    "unix:///var/tmp/xmms-ipc-root"

using namespace mf;

PlayAudio::PlayAudio(QWidget *parent) :
	NBackgroundManagedWidget(parent)
{
	setupUi(this);

	setPreferredBackground(BackgroundVideoOnly);

	m_xmmsClient = new Xmms::Client("Mediafly");
	connect();
}

void PlayAudio::quit()
{
	disconnect();

	try
	{
		Xmms::Client client("mediafly-quit");
		client.connect(XMMS2_CONNECTION_PATH);
		client.playback.stop();
	}
	catch (...)
	{
		qDebug()<< "Stoped server error!";
	}
}

bool PlayAudio::connect()
{
	if (m_xmmsClient)
	{
		try
		{
			m_xmmsClient->connect(XMMS2_CONNECTION_PATH);
			m_xmmsClient->setMainloop(new NXmmsMainloop(m_xmmsClient->getConnection()));
			qDebug() << "Xmms2 client connected to server!";
			m_xmmsClient->setDisconnectCallback(boost::bind(&PlayAudio::disconnect, this));
			m_xmmsClient->playback.broadcastStatus()(Xmms::bind(&PlayAudio::handleStatusChanged, this));
			m_xmmsClient->playback.signalPlaytime()(Xmms::bind(&PlayAudio::handlePlaytime, this));
			m_xmmsClient->playback.getPlaytime()(Xmms::bind(&PlayAudio::handlePlaytime, this));
			return true;
		}
		catch(...)
		{
			qDebug() << "Xmms2 client can't connect to server!";
		}
	}
	return false;
}

void PlayAudio::disconnect()
{
	if (m_xmmsClient)
	{
		delete m_xmmsClient;
		m_xmmsClient = NULL;
	}
}

bool PlayAudio::handlePlaytime(const unsigned int &playtime)
{
	if (playtime - m_songPosition > 500)
	{
		m_songPosition = playtime;
		emit stateChange();
	}
	return true;
}

bool PlayAudio::handleStatusChanged(const Xmms::Playback::Status& status)
{
	if ((m_songPosition > 0) && (status == Xmms::Playback::STOPPED))
	{
		m_isFinished = true;
	}
	else
		m_isFinished = false;

	emit stateChange();
	return true;
}

bool PlayAudio::show(const QModelIndex& index, QString& err)
{
	hide();

	m_index = index;

	QObject::connect(m_index.model(), SIGNAL(refreshed()),
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

	m_songPosition = 0;
	m_songLength = 0;
	m_isFinished = false;

#ifndef NO_FUSE
	// Unmount mount point and stop playing for a case we already
	// play any audio currently.

	umountUrl();

	// Mount httpfs filesystem with given url.

	if (mountUrl(url, err, 130) == false)
	{
		// Mount failed. Well, most probably because server
		// doesn't advertise Conten-Length: in GET reply so it
		// most probably doesn't support Range: either.
		//
		// Use normal way,let xmms2 to manage it completly ->
		// no seek available...
#endif
		url = m_index.data(mf::EpisodeModel::urlRole).toString();
#ifndef NO_FUSE
	}
	else
		url = "file://" + url;
#endif
	if (m_xmmsClient)
	{
		m_xmmsClient->playlist.addUrl(url.toAscii().data());
		m_xmmsClient->playlist.listEntries()(Xmms::bind(&PlayAudio::handlePlaylist, this));
	}
	return true;
}

void PlayAudio::play()
{
	if (m_xmmsClient)
		m_xmmsClient->playback.start();
}

void PlayAudio::pause()
{
	if (m_xmmsClient)
		m_xmmsClient->playback.pause();
}

void PlayAudio::updateImage()
{
	if (m_iconLabel->pixmap()->isNull()) {
		m_iconLabel->setPixmap(m_index.data(mf::EpisodeModel::imageRole).value<QPixmap>());
	}
}

void PlayAudio::hide()
{
	if (m_xmmsClient)
	{
		m_xmmsClient->playback.stop();
		m_xmmsClient->playlist.clear();
	}
}

void PlayAudio::getState(int& songPosition, int& songLength, bool& isFinished)
{
	songPosition = m_songPosition;
	songLength = m_songLength;
	isFinished = m_isFinished;
}

bool PlayAudio::handlePlaylist(const Xmms::List<unsigned int> &list)
{
	m_xmmsClient->medialib.getInfo(*list)(boost::bind(&PlayAudio::playqueueGetMediaInfo, this, _1));
	return true;
}

void PlayAudio::propDictToQHash(const std::string&         key,
                                const Xmms::Dict::Variant& value,
                                const std::string&         source,
                                MusicInfo&                 hash) 
{
	Q_UNUSED( source );

	if(value.type () == typeid (int32_t))
	{
		hash.insert (QString::fromLatin1 (key.c_str ()),
					 QVariant (boost::get< int32_t > (value)));
	}
	else
	{
		if(value.type () == typeid (uint32_t))
		{
			hash.insert (QString::fromLatin1 (key.c_str ()),
						 QVariant (boost::get< uint32_t > (value)));
		}
		else
		{
			QString val;
			if(key == "url")
			{
				/* This really is wrong ...*/
				char *tmpUrl = const_cast<char *>(xmmsc_result_decode_url (NULL, boost::get< std::string >(value).c_str ()));
				val = QString::fromUtf8 (tmpUrl);
				if(val.isEmpty ())
				{
					val = QString::fromUtf8 (tmpUrl);
				}
				free (tmpUrl);
			}
			else
			{
				val = QString::fromUtf8 (boost::get< std::string > (value).c_str ());
			}
			hash.insert (QString::fromUtf8 (key.c_str ()), QVariant (val));
		}
	}
}

bool PlayAudio::playqueueGetMediaInfo(const Xmms::PropDict &info)
{
	MusicInfo hash;

	info.each(boost::bind (&PlayAudio::propDictToQHash,_1, _2, _3, boost::ref (hash)));
	m_songLength = hash["duration"].toInt();
	emit stateChange();
	return true;
}

void PlayAudio::seek(int sec)
{
	if (m_xmmsClient)
	{
		int seekTo = m_songPosition + sec * 1000;
		if (seekTo < 0)
			seekTo = 0;
		m_xmmsClient->playback.seekMs(seekTo);
	}
}

void PlayAudio::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_PageUp:
	{
		if (QScrollBar* bar = m_showDetailsLabel->verticalScrollBar())
			bar->triggerAction(QAbstractSlider::SliderPageStepSub);
		break;
	}
	case Qt::Key_PageDown:
	{
		if (QScrollBar* bar = m_showDetailsLabel->verticalScrollBar())
			bar->triggerAction(QAbstractSlider::SliderPageStepAdd);
		break;
	}
	}
}

