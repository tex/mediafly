#include "MediaflyMenuModel.h"
#include "MediaflyChannelModel.h"
#include "MediaflyEpisodeModel.h"
#include "MediaflyEpisodeDelegate.h"
#include "ui_MediaflyMenu.h"
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class MediaflyMenu : public QWidget, protected Ui::MediaflyMenu
{
	Q_OBJECT
public:
	MediaflyMenu(MediaflyMenuModel&     menuModel,
	             MediaflyChannelModel&  channelModel,
	             MediaflyEpisodeModel&  episodeModel,
	             QWidget               *parent = 0); 

	void showChannelsMenu();

signals:
	void showPlayMenu(const QModelIndex& index);
	void showShowMenu(const QModelIndex& index);

private:
	enum State
	{
		Menu,
		ChannelMenu,
		EpisodeMenu,
	};

	QString	         m_channelSlug;
	State	         m_state;

	MediaflyMenuModel&    m_menuModel;
	MediaflyChannelModel& m_channelModel;
	MediaflyEpisodeModel& m_episodeModel;

	static const int itemsReadAtOnce = 10;

	void selectMenu(QModelIndex& index);
	void renderMenu(const QModelIndex& index);
	void renderEpisodeMenu(const QModelIndex& index);
	void renderChannelMenu(const QModelIndex& index);
	void render(const QModelIndex& index);

	QAbstractItemDelegate   *m_itemDelegateDefault;
	MediaflyEpisodeDelegate *m_itemDelegateEpisode;

	QModelIndex              m_lastMenuIndex;
	QModelIndex              m_lastChannelMenuIndex;

private slots:
	void handleEnterKey();
	void handleRightKey();
	void handleLeftKey();
	void uploadNextPartOfMenu();

private slots:
	void updateChannelModel();
	void updateEpisodeModel();
	void errorHandler(const QString& errorMsg);
};

