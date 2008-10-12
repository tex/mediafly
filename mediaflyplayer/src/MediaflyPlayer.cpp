#include "MediaflyPlayer.h"
#include <QDebug>

MediaflyPlayer::MediaflyPlayer(QWidget *parent) :
	QWidget(parent),
	m_episodeDetails(this),
	m_menu(this, m_menuModel, m_channelModel, m_episodeModel)
{
	m_view.addWidget(&m_menu);
	m_view.addWidget(&m_episodeDetails);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(&m_view);
	setLayout(layout);

	connect(&m_menu, SIGNAL(showShowMenu(const QModelIndex&)),
	        this, SLOT(handleShowMenu(const QModelIndex&)));

	connect(&m_episodeDetails, SIGNAL(back()),
	        this, SLOT(handleEpisodeDetailsBack()));
}

void MediaflyPlayer::handleShowMenu(const QModelIndex& index)
{
	qDebug() << __PRETTY_FUNCTION__;

	m_view.setCurrentWidget(&m_episodeDetails);
	m_episodeDetails.show(index);
}

void MediaflyPlayer::handleEpisodeDetailsBack()
{
	m_view.setCurrentWidget(&m_menu);
}

