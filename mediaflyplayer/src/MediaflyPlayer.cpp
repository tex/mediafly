#include "MediaflyPlayer.h"
#include <QDebug>

MediaflyPlayer::MediaflyPlayer(QWidget *parent) :
	QWidget(parent)
{
	m_episodeDetails = new MediaflyEpisodeDetails(this);
	m_menu = new MediaflyMenu(m_menuModel, m_channelModel, m_episodeModel, this);
	m_play = new MediaflyPlay(this);

	m_view = new QStackedWidget(this);
	m_layout = new QVBoxLayout(this);

	m_view->addWidget(m_menu);
	m_view->addWidget(m_episodeDetails);
	m_view->addWidget(m_play);

	m_layout->addWidget(m_view);
	setLayout(m_layout);

	connect(m_menu, SIGNAL(showShowMenu(const QModelIndex&)),
	        this, SLOT(handleShowMenu(const QModelIndex&)));
	connect(m_menu, SIGNAL(showPlayMenu(const QModelIndex&)),
	        this, SLOT(handlePlayMenu(const QModelIndex&)));

	connect(m_episodeDetails, SIGNAL(showPlayMenu(const QModelIndex&)),
	        this, SLOT(handlePlayMenu(const QModelIndex&)));
	connect(m_episodeDetails, SIGNAL(back()),
	        this, SLOT(showMenu()));

	connect(m_play, SIGNAL(back()),
	        this, SLOT(showMenu()));
	connect(m_play, SIGNAL(backToChannelsMenu()),
	        this, SLOT(showChannelsMenu()));
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

void MediaflyPlayer::showMenu()
{
	m_view->setCurrentWidget(m_menu);
}

void MediaflyPlayer::showChannelsMenu()
{
	m_menu->showChannelsMenu();
	m_view->setCurrentWidget(m_menu);
}

