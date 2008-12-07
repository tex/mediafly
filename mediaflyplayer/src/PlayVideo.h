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

#ifndef mfPlayVideo_H
#define mfPlayVideo_H

#include "ui_MediaflyPlayVideo.h"
#include <QModelIndex>
#include <QTimer>
#include "nmscontrol.h"

namespace mf {

class PlayVideo : public QWidget, private Ui::MediaflyPlayVideo
{
	Q_OBJECT
public:
	PlayVideo(QWidget *parent = 0);
	~PlayVideo();

	void show(const QModelIndex& index);
	void hide();
	void play();
	void pause();

	void getState(int& songPosition, int& songLength);

signals:
	void stateChange();

private:
	QModelIndex m_index;
	long        m_songPosition;
	long        m_songLength;
	NMediaInfo  m_mediaInfo;

	NmsControl *m_nmsControl;
	QTimer     *m_timer;

	void setUrl(QString url);
	QString toTime(unsigned int msec) const;

private slots:
	void handleTimeout();
};

}

#endif

