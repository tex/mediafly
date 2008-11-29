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

#ifndef mfPlayAudio_H
#define mfPlayAudio_H

// XMMS2's headers MUST be included first. Don't forget to include MediaflyPlayAudio.h
// first too if used in another headers. And don't forget to include another headers that
// include this header first...

#include <xmmsclient/xmmsclient++.h>

#include "ui_MediaflyPlayAudio.h"
#include <QModelIndex>
#include <QHash>
#include <QVariant>

namespace mf {

class PlayAudio : public QWidget, private Ui::MediaflyPlayAudio
{
	Q_OBJECT	
public:
	PlayAudio(QWidget *parent = 0);
	~PlayAudio();

	void show(const QModelIndex& index);
	void hide();
	void play();
	void pause();

	void getState(QString& songPosition, QString& songLength);

signals:
	void stateChange();

private:
	QModelIndex m_index;

	typedef QHash<QString, QVariant> MusicInfo;

	void setUrl(QString url);
	void disconnect();
	bool connect();
	bool handlePlaytime(const unsigned int& playtime);
	bool playqueueGetMediaInfo(const Xmms::PropDict &info);
	bool handlePlaylist(const Xmms::List<unsigned int> &list);
	bool handleStatusChanged(const Xmms::Playback::Status& status);
	static void propDictToQHash(const std::string&         key,
	                            const Xmms::Dict::Variant& value,
	                            const std::string&         source,
	                            MusicInfo&                 musicInfo);

	Xmms::Client *m_xmmsClient;
	unsigned int  m_songPosition;
	unsigned int  m_songLength;

private slots:
	void updateImage();
};

}

#endif

