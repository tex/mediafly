#include "MediaflyMenu.h"
#include "MediaflyEpisodeDetails.h"
#include <QMessageBox>
#include <QtDebug>
#include <QKeyEvent>
#include <QIcon>

MediaflyMenu::MediaflyMenu(MediaflyMenuModel& menuModel,
                           MediaflyChannelModel& channelModel,
                           MediaflyEpisodeModel& episodeModel,
                           QWidget *parent) :
	QWidget(parent),
	m_state(Menu),
	m_menuModel(menuModel),
	m_channelModel(channelModel),
	m_episodeModel(episodeModel),
	m_channelLabel(tr("Media Channels"))
{
	setupUi(this);

	connect(m_listView, SIGNAL(almostAtEndOfList()),
	        this, SLOT(uploadNextPartOfMenu()));
	connect(m_listView, SIGNAL(enterPressed()),
	        this, SLOT(handleEnterKey()));
	connect(m_listView, SIGNAL(rightPressed()),
	        this, SLOT(handleRightKey()));
	connect(m_listView, SIGNAL(leftPressed()),
	        this, SLOT(handleLeftKey()));

	connect(&m_menuModel, SIGNAL(refreshed()),
	        this, SLOT(updateMenuModel()));

	connect(&m_channelModel, SIGNAL(refreshed()),
	        this, SLOT(updateChannelModel()));

	connect(&m_episodeModel, SIGNAL(refreshed()),
	        this, SLOT(updateEpisodeModel()));

	connect(Mediafly::getMediafly(), SIGNAL(readError(const QString&)),
	        this, SLOT(errorHandler(const QString&)));

	connect(&m_setUserAsDefaultData, SIGNAL(ready()),
	        this, SLOT(showChannelMenu()));

	connect(&m_unbindMFUserData, SIGNAL(done()),
	        this, SLOT(showChannelMenu()));

	// Remember the default item delegate that m_listView uses.
	// 
	m_itemDelegateDefault = m_listView->itemDelegate();
	m_itemDelegateEpisode = new MediaflyEpisodeDelegate(m_listView);

	m_lastMenuIndex = m_menuModel.index(0, 0);
	m_lastChannelMenuIndex = QModelIndex();

	m_menuModel.refresh();

	render(QModelIndex());
}

void MediaflyMenu::updateMenuModel()
{
	qDebug() << __PRETTY_FUNCTION__;

	QAbstractItemModel *model = m_listView->model();
	if (model != &m_menuModel)
		return;

	// Remember current selected index (position)
	// if episode menu is already shown.

	if (m_listView->currentIndex().isValid()) {
		m_lastMenuIndex = m_listView->currentIndex();
	} else if (!m_lastMenuIndex.isValid()) {
		m_lastMenuIndex = m_menuModel.index(0, 0);
	}

	m_listView->setModel(NULL);
	m_listView->setModel(model);

	// setModel itself doesn't "refresh", we
	// have to call 'update' to repaint it and
	// 'setCurrentIndex' to select the first item.

	m_listView->update(m_lastMenuIndex);
	m_listView->setCurrentIndex(m_lastMenuIndex);
}

void MediaflyMenu::updateChannelModel()
{
	qDebug() << __PRETTY_FUNCTION__;

	QAbstractItemModel *model = m_listView->model();
	if (model != &m_channelModel)
		return;

	// Remember current selected index (position)
	// if episode menu is already shown.

	if (m_listView->currentIndex().isValid()) {
		m_lastChannelMenuIndex = m_listView->currentIndex();
	} else if (!m_lastChannelMenuIndex.isValid()) {
		m_lastChannelMenuIndex = m_channelModel.index(0, 0);
	}

	m_listView->setModel(NULL);
	m_listView->setModel(model);

	// setModel itself doesn't "refresh", we
	// have to call 'update' to repaint it and
	// 'setCurrentIndex' to select the first item.

	m_listView->update(m_lastChannelMenuIndex);
	m_listView->setCurrentIndex(m_lastChannelMenuIndex);
}

void MediaflyMenu::updateEpisodeModel()
{
	qDebug() << __PRETTY_FUNCTION__;

	QAbstractItemModel *model = m_listView->model();
	if (model != &m_episodeModel)
		return;

	QModelIndex current = m_listView->currentIndex();
	if (!current.isValid())
		current = m_episodeModel.index(0, 0);

	m_listView->setModel(NULL);
	m_listView->setModel(model);

	m_listView->update(current);
	m_listView->setCurrentIndex(current);
}

void MediaflyMenu::errorHandler(const QString& errorMsg)
{
	QMessageBox::critical(this, "Error", errorMsg);

	// In case of an error switch to main menu. It
	// may be used to leave the application.

	m_state = Menu;
	render(QModelIndex());
}

void MediaflyMenu::renderMenu(const QModelIndex& /*index*/)
{
	m_header->setText(tr("Mediafly"));
	m_icon->setVisible(true);

	m_channelModel.cancel();

	m_listView->setItemDelegate(m_itemDelegateDefault);

	m_listView->setModel(NULL);
	m_listView->setModel(&m_menuModel);

	m_listView->setCurrentIndex(m_lastMenuIndex);

	m_listView->setEnabled(true);
}

void MediaflyMenu::renderEpisodeMenu(const QModelIndex& index)
{
	m_header->setText(tr("Media Episodes"));
	m_icon->setVisible(false);

	m_episodeModel.cancel();
	m_episodeModel.clear();

	QString slug = index.data(MediaflyChannelModel::slugRole).toString();
	QString name = index.data(MediaflyChannelModel::nameRole).toString();

	m_channelSlug = slug;
	m_episodeModel.refresh(MediaflyEpisodeQuery(slug, 0, itemsReadAtOnce));

	m_listView->setModel(&m_episodeModel);
	m_listView->setItemDelegate(m_itemDelegateEpisode);
}

void MediaflyMenu::renderChannelMenu(const QModelIndex& /*index*/)
{
	m_header->setText(m_channelLabel);
	m_icon->setVisible(true);

	m_episodeModel.cancel();
	m_channelModel.refresh();

	m_listView->setModel(&m_channelModel);
	m_listView->setItemDelegate(m_itemDelegateDefault);

	m_listView->setCurrentIndex(m_lastChannelMenuIndex);
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
	default:
		QMessageBox::information(this, tr("Missing feature"), tr("Not yet implemented"));
		break;
	case MediaflyMenuModel::MENU_MEDIA_CHANNELS:
		m_state = ChannelMenu;
		break;
	case MediaflyMenuModel::MENU_USER:
	{
		// Set correct episode menu's label.

		QString accountName = index.data(MediaflyMenuModel::nameRole).toString();
		m_channelLabel = accountName + tr("'s Mediafly");

		// If selected user not default already, make him default...

		if (index.data(MediaflyMenuModel::isDefaultRole).toBool() == false)
		{
			// m_setUserAsDefaultData (ready()) signal's callback (showChannelMenu())
			// sets m_state to ChannelMenu and renders the menu when we sucessfully
			// get return notification.

			Mediafly::getMediafly()->Authentication_SetMFUserAsDefault(&m_setUserAsDefaultData, accountName);

			// Remove model to let the MediaflyList render 'Loading menu... Please wait' message.
			// Channel menu will be shown by calling slot showChannelMenu()...

			m_listView->setModel(NULL);
		}
		else
			// Already set as default. Just proceed to ChannelMenu and it will reads
			// the user's menu.
			//
			m_state = ChannelMenu;

		break;
	}
	case MediaflyMenuModel::MENU_PERSONALIZE:
		emit showPersonalize();
		break;
	case MediaflyMenuModel::MENU_ADD_PERSON:
		emit showLoginPerson();
		break;
	case MediaflyMenuModel::MENU_REMOVE_PERSON:
	{
		Mediafly::getMediafly()->Channels_UnbindMFUser(&m_unbindMFUserData);

		// Remove model to let the MediaflyList render 'Loading menu... Please wait' message.
		// Channel menu will be shown by calling slot showChannelMenu()...

		m_listView->setModel(NULL);
		break;
	}
	}
}

void MediaflyMenu::handleEnterKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView->currentIndex();
	if (!index.isValid())
		return;

	switch (m_state) {
	case Menu:
		m_lastMenuIndex = index;
		selectMenu(index);
		break;
	case ChannelMenu:
		m_lastChannelMenuIndex = index;
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

	QModelIndex index = m_listView->currentIndex();
	if (!index.isValid())
		return;

	switch (m_state) {
	case Menu:
		m_lastMenuIndex = index;
		selectMenu(index);
		break;
	case ChannelMenu:
		m_lastChannelMenuIndex = index;
		m_state = EpisodeMenu;
		break;
	case EpisodeMenu:
	{
		emit showShowMenu(index);
		return;
	}
	default:
		return;
	}
	render(index);
}

void MediaflyMenu::handleLeftKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView->currentIndex();

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
		m_episodeModel.refresh(MediaflyEpisodeQuery(m_channelSlug, m_episodeModel.rowCount(), itemsReadAtOnce));
		return;
	default:
		return;
	}
}

void MediaflyMenu::showChannelMenu()
{
	m_state = ChannelMenu;
	render(QModelIndex());
}

