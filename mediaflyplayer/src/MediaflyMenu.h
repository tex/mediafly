#include "MenuModel.h"
#include "ChannelModel.h"
#include "EpisodeModel.h"
#include "EpisodeDelegate.h"
#include "MediaflyCheckResponseOk.h"
#include "ui_MediaflyMenu.h"
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class MediaflyMenu : public QWidget, protected Ui::MediaflyMenu
{
	Q_OBJECT
public:
	MediaflyMenu(mf::MenuModel&         menuModel,
	             mf::ChannelModel&      channelModel,
	             mf::EpisodeModel&      episodeModel,
	             QWidget               *parent = 0); 

	void showChannelsMenu();

signals:
	void showPlayMenu(const QModelIndex& index);
	void showShowMenu(const QModelIndex& index);
	void showPersonalize();
	void showLoginPerson();

public slots:
	void showChannelMenu();

private:
	enum State
	{
		Menu,
		ChannelMenu,
		EpisodeMenu,
	};

	QString	         m_channelSlug;
	State	         m_state;

	mf::MenuModel&        m_menuModel;
	mf::ChannelModel&     m_channelModel;
	mf::EpisodeModel&     m_episodeModel;

	static const int itemsReadAtOnce = 10;

	void selectMenu(QModelIndex& index);
	void renderMenu(const QModelIndex& index);
	void renderEpisodeMenu(const QModelIndex& index);
	void renderChannelMenu(const QModelIndex& index);
	void render(const QModelIndex& index);

	QAbstractItemDelegate   *m_itemDelegateDefault;
	mf::EpisodeDelegate     *m_itemDelegateEpisode;

	QModelIndex              m_lastMenuIndex;
	QModelIndex              m_lastChannelMenuIndex;

	mf::auth::SetMFUserAsDefaultData              m_setUserAsDefaultData;
	MediaflyCheckResponseOk                       m_checkResponseOk;

	QString m_channelLabel;

private slots:
	void handleEnterKey();
	void handleRightKey();
	void handleLeftKey();
	void uploadNextPartOfMenu();

private slots:
	void updateMenuModel();
	void updateChannelModel();
	void updateEpisodeModel();
	void errorHandler(const QString& errorMsg);
};

