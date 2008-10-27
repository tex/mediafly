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

#ifndef mfPlayer_H
#define mfPlayer_H

#include "Play.h"
#include "EpisodeDetails.h"
#include "Menu.h"
#include "PersonalizeSimple.h"
#include "Playqueue.h"
#include "Search.h"
#include "nbackgroundmanagedwidget.h"
#include "nstdinappmanager.h"
#include <QWidget>
#include <QObject>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace mf {

class Player : public NBackgroundManagedWidget
{
	Q_OBJECT
public:
	Player(QWidget *parent = 0);

private:
	QVBoxLayout    *m_layout;
	QStackedWidget *m_view;

	MenuModel            m_menuModel;
	ChannelModel         m_channelModel;
	EpisodeModel         m_episodeModel;

	EpisodeDetails         *m_episodeDetails;
	Menu                   *m_menu;
	Play                   *m_play;
	PersonalizeSimple      *m_personalize;
	Playqueue              *m_playqueue;
	Search                 *m_search;

	NStdinAppManager stdinman;

private slots:
	void handleSearchTerm(QString term);
	void handleSearch();
	void handlePlayMenu(const QModelIndex& index);
	void handleShowMenu(const QModelIndex& index);
	void handlePersonalize();
	void handleNewPerson();
	void showMenu();
	void showMainMenu();
	void showChannelsMenu();
	void showPlayqueue();
	void showPlay();
	void onQuit();
};

}

#endif

