#include "MediaflyEpisodeDetails.h"
#include "MediaflyMenu.h"
#include "MediaflyPlay.h"
#include "MediaflyPersonalize.h"
#include "MediaflyLoginPerson.h"
#include <QWidget>
#include <QObject>
#include <QStackedWidget>
#include <QVBoxLayout>

class MediaflyPlayer : public QWidget
{
	Q_OBJECT
public:
	MediaflyPlayer(QWidget *parent = 0);

private:
	QVBoxLayout    *m_layout;
	QStackedWidget *m_view;

	MediaflyMenuModel    m_menuModel;
	MediaflyChannelModel m_channelModel;
	MediaflyEpisodeModel m_episodeModel;

	MediaflyEpisodeDetails *m_episodeDetails;
	MediaflyMenu           *m_menu;
	MediaflyPlay           *m_play;
	MediaflyPersonalize    *m_personalize;
	MediaflyLoginPerson    *m_loginPerson;

private slots:
	void handlePlayMenu(const QModelIndex& index);
	void handleShowMenu(const QModelIndex& index);
	void handlePersonalize();
	void handleLoginPerson();
	void handleNewPerson();
	void showMenu();
	void showChannelsMenu();
};

