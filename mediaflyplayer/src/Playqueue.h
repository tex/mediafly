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

#ifndef Playqueue_H
#define Playqueue_H

#include "nbackgroundmanagedwidget.h"
#include "ui_MediaflyPlayqueue.h"
#include "EpisodeModel.h"
#include "EpisodeDelegate.h"
#include "CheckResponseOk.h"
#include <QWidget>

namespace mf {

class Play;

class Playqueue : public NBackgroundManagedWidget, private Ui::MediaflyPlayqueue
{
	Q_OBJECT
public:
	Playqueue(EpisodeModel& episodeModel, mf::Play *mediaflyPlay, QWidget *parent = 0);

signals:
	void back();

private:
	void keyPressEvent(QKeyEvent *event);

	EpisodeModel             &m_episodeModel;
	mf::Play                 *m_mediaflyPlay;
	EpisodeDelegate          *m_itemDelegate;
	mf::CheckResponseOk       m_checkResponseOk;

	static const int itemsReadAtOnce = 10;

private slots:
	void uploadNextPartOfMenu();
	void updateEpisodeModel();
	void handleNowPlayingClicked();
	void handleRemoveButtonClicked();
	void handleCheckResponseOkDone();
	void handleStateChange();
};

}

#endif

