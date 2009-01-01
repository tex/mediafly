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

#include "Player.h"
#include "nmscontrol.h"
#include "nssaverchannel.h"

#include <QDebug>

using namespace mf;

Player::Player(QWidget *parent) :
	NBackgroundManagedWidget(parent)
{
	setPreferredBackground(BackgroundVideoOnly);

	QApplication::setQuitOnLastWindowClosed(true);
	connect(&stdinman, SIGNAL(quit()), this, SLOT(onQuit()));

	m_episodeDetails = new mf::EpisodeDetails(this);
	m_menu = new mf::Menu(m_menuModel, m_channelModel, m_episodeModel, this);
	m_play = new mf::Play(this);
	m_personalize = new mf::PersonalizeSimple(this);
	m_playqueue = new mf::Playqueue(m_episodeModel, m_play, this);
	m_search = new mf::Search(this);

	m_view = new QStackedWidget(this);
	m_layout = new QVBoxLayout(this);

	m_view->addWidget(m_menu);
	m_view->addWidget(m_episodeDetails);
	m_view->addWidget(m_play);
	m_view->addWidget(m_personalize);
	m_view->addWidget(m_playqueue);
	m_view->addWidget(m_search);

	m_layout->addWidget(m_view);
	setLayout(m_layout);

	connect(m_menu, SIGNAL(showShowMenu(const QModelIndex&)),
	        this, SLOT(handleShowMenu(const QModelIndex&)));
	connect(m_menu, SIGNAL(showPlayMenu(const QModelIndex&)),
	        this, SLOT(handlePlayMenu(const QModelIndex&)));
	connect(m_menu, SIGNAL(showPersonalize()),
	        this, SLOT(handlePersonalize()));
	connect(m_menu, SIGNAL(showSearch()),
	        this, SLOT(handleSearch()));

	connect(m_episodeDetails, SIGNAL(showPlayMenu(const QModelIndex&)),
	        this, SLOT(handlePlayMenu(const QModelIndex&)));
	connect(m_episodeDetails, SIGNAL(back()),
	        this, SLOT(showMenu()));

	connect(m_play, SIGNAL(back()),
	        this, SLOT(showMenu()));
	connect(m_play, SIGNAL(backToChannelsMenu()),
	        this, SLOT(showChannelsMenu()));
	connect(m_play, SIGNAL(showPlayqueue()),
	        this, SLOT(showPlayqueue()));

	connect(m_personalize, SIGNAL(back()),
	        this, SLOT(showMenu()));

	connect(m_playqueue, SIGNAL(back()),
	        this, SLOT(showPlay()));

	connect(m_search, SIGNAL(back()),
	        this, SLOT(showMainMenu()));
	connect(m_search, SIGNAL(search(QString)),
	        this, SLOT(handleSearchTerm(QString)));
}

void Player::handleNewPerson()
{
	m_menuModel.refresh();
	showMenu();
}

Player::~Player()
{
}

void Player::onQuit()
{
	foreach (QWidget *widget, QApplication::topLevelWidgets())
	{
		widget->close();
	}
}

void Player::handleShowMenu(const QModelIndex& index)
{
	m_view->setCurrentWidget(m_episodeDetails);
	m_episodeDetails->show(index);
}

void Player::handlePlayMenu(const QModelIndex& index)
{
	m_view->setCurrentWidget(m_play);
	m_play->show(index);
}

void Player::showPlay()
{
	m_view->setCurrentWidget(m_play);
}

void Player::handlePersonalize()
{
	m_view->setCurrentWidget(m_personalize);
	m_personalize->show();
}

void Player::handleSearch()
{
	m_view->setCurrentWidget(m_search);
	m_search->show();
}

void Player::handleSearchTerm(QString term)
{
	qDebug() << __PRETTY_FUNCTION__ << term;
	m_view->setCurrentWidget(m_menu);

	SearchQuery query(term);
	m_menu->showEpisodeMenu(query);
}

void Player::showMenu()
{
	m_view->setCurrentWidget(m_menu);
}

void Player::showMainMenu()
{
	m_menu->showMainMenu();
	m_view->setCurrentWidget(m_menu);
}

void Player::showChannelsMenu()
{
	m_menu->showChannelMenu();
	m_view->setCurrentWidget(m_menu);
}

void Player::showPlayqueue()
{
	m_view->setCurrentWidget(m_playqueue);
}

