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

#ifndef MediaflyPlay_H
#define MediaflyPlay_H

#include "PlayAudio.h"
#include "PlayVideo.h"
#include "ui_MediaflyPlay.h"
#include "CheckResponseOk.h"
#include "nmediakeychannel.h"
#include <QModelIndex>
#include <QKeyEvent>

namespace mf {

class Play : public QWidget, protected Ui::MediaflyPlay
{
	Q_OBJECT
public:
	Play(QWidget *parent = 0);

	void show(const QModelIndex& index);
	void quit();
	void getState(QModelIndex &currentIndex, QString &songPosition, QString &songLength);

signals:
	void back();
	void backToChannelMenu();

private:
	enum State
	{
		MP_PAUSE,
		MP_PLAY,
		MP_ERROR,
	}
	m_state;

	QModelIndex             m_index;
	mf::CheckResponseOk     m_checkResponseOk;
	NMediaKeyClient         m_mediakeyChannel;
	PlayAVInterface        *m_output;
	bool                    m_ready;

	void keyPressEvent(QKeyEvent *event);
	void update();
	void updateState();
	QString toTime(unsigned int msec) const;

private slots:
	void handleNextEpisodeButtonClicked();
	void handlePreviousEpisodeButtonClicked();
	void handlePlayStateButtonClicked();
	void handleStateChange();
};

}

#endif

