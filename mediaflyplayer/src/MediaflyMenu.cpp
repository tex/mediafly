#include "MediaflyMenu.h"
#include <QMessageBox>
#include <QtDebug>
#include <QKeyEvent>

MediaflyMenu::MediaflyMenu(QWidget *parent) :
	QWidget(parent),
	m_listView(this),
	m_state(Menu)
{
	// Remember the default item delegate that m_listView uses.
	// 
	m_itemDelegateDefault = m_listView.itemDelegate();

	m_layout.addWidget(&m_listView);
	setLayout(&m_layout);

	m_listView.setEnabled(false);

	connect(&m_listView, SIGNAL(almostAtEndOfList()),
	        this, SLOT(uploadNextPartOfMenu()));
	connect(&m_listView, SIGNAL(enterPressed()),
	        this, SLOT(handleEnterKey()));
	connect(&m_listView, SIGNAL(rightPressed()),
	        this, SLOT(handleRightKey()));
	connect(&m_listView, SIGNAL(leftPressed()),
	        this, SLOT(handleLeftKey()));

	connect(&m_channelModel, SIGNAL(refreshed()),
	        this, SLOT(updateChannelModel()));
	connect(&m_channelModel, SIGNAL(error(const QString&)),
	        this, SLOT(errorHandler(const QString&)));

	connect(&m_episodeModel, SIGNAL(entryRefreshed()),
	        this, SLOT(updateEpisodeModel()));
	connect(&m_episodeModel, SIGNAL(imageRefreshed()),
	        this, SLOT(updateEpisodeModel()));
	connect(&m_episodeModel, SIGNAL(error(const QString&)),
	        this, SLOT(errorHandler(const QString&)));

	render(QModelIndex());
}

void MediaflyMenu::updateChannelModel()
{
	qDebug() << __PRETTY_FUNCTION__;

	m_listView.setItemDelegate(m_itemDelegateDefault);

	// Remember current selected index (position)
	// if episode menu is already shown.

	QModelIndex current = m_channelModel.index(0, 0);
	if (m_listView.model() == &m_channelModel)
		current = m_listView.currentIndex();

	m_listView.setModel(NULL);
	m_listView.setModel(&m_channelModel);

	// setModel itself doesn't "refresh", we
	// have to call 'update' to repaint it and
	// 'setCurrentIndex' to select the first item.

	m_listView.update(current);
	m_listView.setCurrentIndex(current);

	m_listView.setEnabled(true);
	m_listView.setFocus();
}

void MediaflyMenu::updateEpisodeModel()
{
	qDebug() << __PRETTY_FUNCTION__;

	m_listView.setItemDelegate(&m_itemDelegateEpisode);

	// Remember current selected index (position)
	// if episode menu is already shown.

	QModelIndex current = m_episodeModel.index(0, 0);
	if (m_listView.model() == &m_episodeModel)
		current = m_listView.currentIndex();

	m_listView.setModel(NULL);
	m_listView.setModel(&m_episodeModel);

	// setModel itself doesn't "refresh", we
	// have to call 'update' to repaint it and
	// 'setCurrentIndex' to select the first item.

	m_listView.update(current);
	m_listView.setCurrentIndex(current);

	m_listView.setEnabled(true);
	m_listView.setFocus();
}

void MediaflyMenu::errorHandler(const QString& errorMsg)
{
	QMessageBox::critical(this, "Error", errorMsg);

	// In case of an error switch to main menu. This
	// menu doesn't require internet connection and it
	// may be used to leave the application.

	m_state = Menu;
	render(QModelIndex());
}

void MediaflyMenu::renderMenu(const QModelIndex& /*index*/)
{
	m_listView.setItemDelegate(m_itemDelegateDefault);

	m_listView.setModel(NULL);
	m_listView.setModel(&m_menuModel);

	m_listView.update(m_lastMenuIndex);
	m_listView.setCurrentIndex(m_lastMenuIndex);

	m_listView.setEnabled(true);
	m_listView.setFocus();
}

void MediaflyMenu::renderEpisodeMenu(const QModelIndex& index)
{
	m_episodeModel.clear();

	QString slug = index.data(MediaflyChannelModel::slugRole).toString();
	QString name = index.data(MediaflyChannelModel::nameRole).toString();

	m_channelSlug = slug;
	m_episodeModel.refresh(slug, 0, itemsReadAtOnce);

	m_state = EpisodeMenu;
	m_listView.setEnabled(false);
}

void MediaflyMenu::renderChannelMenu(const QModelIndex& /*index*/)
{
	m_channelModel.refresh();

	m_state = ChannelMenu;
	m_listView.setEnabled(false);
}

void MediaflyMenu::render(const QModelIndex& index)
{
	switch (m_state) {
	case Menu:
		renderMenu(index);
		break;
	case ChannelMenu:
		renderChannelMenu(index);
		break;
	case EpisodeMenu:
		renderEpisodeMenu(index);
		break;
	default:
		Q_ASSERT(false);
	}
}

void MediaflyMenu::selectMenu(QModelIndex& index)
{
	switch (index.data(MediaflyMenuModel::slugRole).toInt()) {
	case MediaflyMenuModel::MENU_SEARCH:
	case MediaflyMenuModel::MENU_POPULAR_CHANNELS:
	case MediaflyMenuModel::MENU_PERSONALIZE:
	default:
		QMessageBox::information(this, "Missing feature", "Not yet implemented");
		break;
	case MediaflyMenuModel::MENU_MEDIA_CHANNELS:
		m_state = ChannelMenu;
		break;
	}
}

void MediaflyMenu::handleEnterKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView.currentIndex();

	switch (m_state) {
	case Menu:
		selectMenu(index);
		break;
	case ChannelMenu:
		m_state = EpisodeMenu;
		break;
	case EpisodeMenu:
		emit showPlayMenu(index);
		return;
	default:
		return;
	}
	render(index);
}

void MediaflyMenu::handleRightKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView.currentIndex();

	switch (m_state) {
	case Menu:
		selectMenu(index);
		break;
	case ChannelMenu:
		m_state = EpisodeMenu;
		break;
	case EpisodeMenu:
		emit showShowMenu(index);
		return;
	default:
		return;
	}
	render(index);
}

void MediaflyMenu::handleLeftKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView.currentIndex();

	switch (m_state) {
	case ChannelMenu:
		m_state = Menu;
		break;
	case EpisodeMenu:
		m_state = ChannelMenu;
		break;
	default:
		return;
	}
	render(index);
}

void MediaflyMenu::uploadNextPartOfMenu()
{
	qDebug() << __PRETTY_FUNCTION__;

	switch (m_state) {
	case EpisodeMenu:
		m_episodeModel.refresh(m_channelSlug, m_episodeModel.rowCount(), itemsReadAtOnce);
		return;
	default:
		return;
	}
}

