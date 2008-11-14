#include "MediaflyPlayer.h"
#include <QDebug>

MediaflyPlayer::MediaflyPlayer(QWidget *parent) :
	QWidget(parent)
{
	m_episodeDetails = new MediaflyEpisodeDetails(this);
	m_menu = new MediaflyMenu(m_menuModel, m_channelModel, m_episodeModel, this);
	m_play = new MediaflyPlay(this);
	m_loginPerson = new MediaflyLoginPerson();
	m_personalize = new MediaflyPersonalize(m_loginPerson);
	m_playqueue = new mf::Playqueue(m_episodeModel, this);

	m_view = new QStackedWidget(this);
	m_layout = new QVBoxLayout(this);

	m_view->addWidget(m_menu);
	m_view->addWidget(m_episodeDetails);
	m_view->addWidget(m_play);
	m_view->addWidget(m_personalize);
	m_view->addWidget(m_loginPerson);
	m_view->addWidget(m_playqueue);

	m_layout->addWidget(m_view);
	setLayout(m_layout);

	connect(m_menu, SIGNAL(showShowMenu(const QModelIndex&)),
	        this, SLOT(handleShowMenu(const QModelIndex&)));
	connect(m_menu, SIGNAL(showPlayMenu(const QModelIndex&)),
	        this, SLOT(handlePlayMenu(const QModelIndex&)));
	connect(m_menu, SIGNAL(showPersonalize()),
	        this, SLOT(handlePersonalize()));
	connect(m_menu, SIGNAL(showLoginPerson()),
	        this, SLOT(handleLoginPerson()));

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

	connect(m_loginPerson, SIGNAL(newPerson()),
	        this, SLOT(handleNewPerson()));
	connect(m_loginPerson, SIGNAL(back()),
	        this, SLOT(showMenu()));

	connect(m_personalize, SIGNAL(showLoginPerson()),
	        this, SLOT(handleLoginPerson()));

	connect(m_playqueue, SIGNAL(back()),
	        this, SLOT(showPlay()));
}

void MediaflyPlayer::handleNewPerson()
{
	m_menuModel.refresh();
	showMenu();
}

void MediaflyPlayer::handleShowMenu(const QModelIndex& index)
{
	m_view->setCurrentWidget(m_episodeDetails);
	m_episodeDetails->show(index);
}

void MediaflyPlayer::handlePlayMenu(const QModelIndex& index)
{
	m_view->setCurrentWidget(m_play);
	m_play->show(index);
}

void MediaflyPlayer::showPlay()
{
	m_view->setCurrentWidget(m_play);
}

void MediaflyPlayer::handlePersonalize()
{
	m_personalize->clear();
	m_view->setCurrentWidget(m_personalize);
}

void MediaflyPlayer::handleLoginPerson()
{
	m_loginPerson->clear();
	m_view->setCurrentWidget(m_loginPerson);
}

void MediaflyPlayer::showMenu()
{
	m_view->setCurrentWidget(m_menu);
}

void MediaflyPlayer::showChannelsMenu()
{
	m_menu->showChannelMenu();
	m_view->setCurrentWidget(m_menu);
}

void MediaflyPlayer::showPlayqueue()
{
	m_view->setCurrentWidget(m_playqueue);
}

