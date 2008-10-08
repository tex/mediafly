#include "MediaflyMenuModel.h"
#include "MediaflyChannelModel.h"
#include "MediaflyEpisodeModel.h"
#include "MediaflyEpisodeDelegate.h"
#include "MediaflyList.h"
#include <QObject>
#include <QVBoxLayout>

class MediaflyMenu : public QWidget
{
	Q_OBJECT
public:
	MediaflyMenu(QWidget *parent = 0); 
	virtual ~MediaflyMenu() { };

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

	QVBoxLayout      m_layout;
	MediaflyList     m_listView;
	QString	         m_channelSlug;
	State	         m_state;

	MediaflyMenuModel    m_menuModel;
	MediaflyChannelModel m_channelModel;
	MediaflyEpisodeModel m_episodeModel;

	static const int itemsReadAtOnce = 10;

	void selectMenu(QModelIndex& index);
	void renderMenu(const QModelIndex& index);
	void renderEpisodeMenu(const QModelIndex& index);
	void renderChannelMenu(const QModelIndex& index);
	void render(const QModelIndex& index);

	QAbstractItemDelegate   *m_itemDelegateDefault;
	MediaflyEpisodeDelegate  m_itemDelegateEpisode;

	QModelIndex              m_lastMenuIndex;
	QModelIndex              m_lastChannelIndex;

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

