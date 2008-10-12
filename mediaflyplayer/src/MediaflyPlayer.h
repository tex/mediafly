#include "MediaflyEpisodeDetails.h"
#include "MediaflyMenu.h"
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

private slots:
	void handleShowMenu(const QModelIndex& index);
	void handleEpisodeDetailsBack();
};

