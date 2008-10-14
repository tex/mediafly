#include "MediaflyPlayer.h"
#include <QDebug>

MediaflyPlayer::MediaflyPlayer(QWidget *parent) :
	QWidget(parent)
{
	m_episodeDetails = new MediaflyEpisodeDetails(this);
	m_menu = new MediaflyMenu(m_menuModel, m_channelModel, m_episodeModel, this);

	m_view = new QStackedWidget(this);
	m_layout = new QVBoxLayout(this);

	m_view->addWidget(m_menu);
	m_view->addWidget(m_episodeDetails);

	m_layout->addWidget(m_view);
	setLayout(m_layout);

	connect(m_menu, SIGNAL(showShowMenu(const QModelIndex&)),
	        this, SLOT(handleShowMenu(const QModelIndex&)));
	connect(m_menu, SIGNAL(showPlayMenu(const QModelIndex&)),
	        this, SLOT(handlePlayMenu(const QModelIndex&)));

	connect(m_episodeDetails, SIGNAL(back()),
	        this, SLOT(handleEpisodeDetailsBack()));
}

void MediaflyPlayer::handleShowMenu(const QModelIndex& index)
{
	qDebug() << __PRETTY_FUNCTION__;

	m_view->setCurrentWidget(m_episodeDetails);
	m_episodeDetails->show(index);
}

void MediaflyPlayer::handlePlayMenu(const QModelIndex& index)
{
	QString url = index.data(MediaflyEpisodeModel::urlRole).toString();
	// TODO
}

void MediaflyPlayer::handleEpisodeDetailsBack()
{
	m_view->setCurrentWidget(m_menu);
}

