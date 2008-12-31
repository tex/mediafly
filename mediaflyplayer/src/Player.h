#ifndef mfPlayer_H
#define mfPlayer_H

#include "Play.h"
#include "MediaflyEpisodeDetails.h"
#include "MediaflyMenu.h"
#include "Personalize.h"
#include "LoginPerson.h"
#include "Playqueue.h"
#include <QWidget>
#include <QObject>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace mf {

class Player : public QWidget
{
	Q_OBJECT
public:
	Player(QWidget *parent = 0);

private:
	QVBoxLayout    *m_layout;
	QStackedWidget *m_view;

	MediaflyMenuModel    m_menuModel;
	MediaflyChannelModel m_channelModel;
	MediaflyEpisodeModel m_episodeModel;

	MediaflyEpisodeDetails *m_episodeDetails;
	MediaflyMenu           *m_menu;
	Play                   *m_play;
	Personalize            *m_personalize;
	LoginPerson            *m_loginPerson;
	Playqueue              *m_playqueue;

private slots:
	void handlePlayMenu(const QModelIndex& index);
	void handleShowMenu(const QModelIndex& index);
	void handlePersonalize();
	void handleLoginPerson();
	void handleNewPerson();
	void showMenu();
	void showChannelsMenu();
	void showPlayqueue();
	void showPlay();
};

}

#endif

