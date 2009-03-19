/*
 *  Copyright(C) 2009 Milan Svoboda. 
 *               <milan.svoboda@centrum.cz>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *  
 *
 *  This program is distributed in the hope that, in addition to its 
 *  original purpose to support Neuros hardware, it will be useful 
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************/

#include "Menu.h"
#include "nmessagebox.h"
#include <QtDebug>
#include <QKeyEvent>
#include <QIcon>

mf::Menu::Menu(mf::MenuModel&        menuModel,
               mf::ChannelModel&     channelModel,
               mf::EpisodeModel&     episodeModel,
               QWidget *parent) :
	QWidget(parent),
	m_state(MainMenu),
	m_menuModel(menuModel),
	m_channelModel(channelModel),
	m_episodeModel(episodeModel),
	m_channelLabel(tr("Media Channels"))
{
	setupUi(this);

	connect(m_listView, SIGNAL(enterPressed()),
	        this, SLOT(handleEnterKey()));
	connect(m_listView, SIGNAL(rightPressed()),
	        this, SLOT(handleRightKey()));
	connect(m_listView, SIGNAL(leftPressed()),
	        this, SLOT(handleLeftKey()));
	connect(m_listView, SIGNAL(exitPressed()),
		this, SLOT(handleExitKey()));

	connect(&m_menuModel, SIGNAL(refreshed()),
	        this, SLOT(updateMenuModel()));

	connect(&m_channelModel, SIGNAL(refreshed()),
	        this, SLOT(updateChannelModel()));

	connect(&m_episodeModel, SIGNAL(refreshed()),
	        this, SLOT(updateEpisodeModel()));

	connect(Mediafly::getMediafly(), SIGNAL(readError(const QString&)),
	        this, SLOT(errorHandler(const QString&)));

	connect(&m_setUserAsDefaultData, SIGNAL(ready()),
	        this, SLOT(handleSetUserAsDefaultReady()));

	connect(&m_checkResponseOk, SIGNAL(done()),
	        &m_menuModel, SLOT(refresh()));

	// Remember the default item delegate that m_listView uses.
	// 
	m_itemDelegateDefault = m_listView->itemDelegate();
	m_itemDelegateEpisode = new mf::EpisodeDelegate(m_listView);

	m_lastMenuIndex = m_menuModel.index(0, 0);
	m_lastChannelMenuIndex = QModelIndex();

	m_menuModel.refresh();

	render(QModelIndex());
}

void mf::Menu::handleSetUserAsDefaultReady()
{
	m_menuModel.refresh();
	showChannelMenu();
}

void mf::Menu::updateMenuModel()
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
	if (m_lastMenuIndex.row() > m_menuModel.rowCount()) {
		m_lastMenuIndex = m_menuModel.index(m_menuModel.rowCount() - 1, 0);
	}

	m_listView->setModel(NULL);
	m_listView->setModel(model);

	// setModel itself doesn't "refresh", we
	// have to call 'update' to repaint it and
	// 'setCurrentIndex' to select the first item.

	m_listView->update(m_lastMenuIndex);
	m_listView->setCurrentIndex(m_lastMenuIndex);
}

void mf::Menu::updateChannelModel()
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

void mf::Menu::updateEpisodeModel()
{
	qDebug() << __PRETTY_FUNCTION__;

	QAbstractItemModel *model = m_listView->model();
	if (model != &m_episodeModel)
		return;

	if (model->rowCount() == 0)
		m_listView->setEmpty();

	QModelIndex current = m_listView->currentIndex();
	if (!current.isValid())
		current = m_episodeModel.index(0, 0);

	m_listView->setModel(NULL);
	m_listView->setModel(model);

	m_listView->update(current);
	m_listView->setCurrentIndex(current);
}

void mf::Menu::errorHandler(const QString& errorMsg)
{
	NMessageBox::critical(this, "Error", errorMsg);

	// In case of an error switch to main menu. It
	// may be used to leave the application.

	m_state = MainMenu;
	render(QModelIndex());
}

void mf::Menu::renderMenu(const QModelIndex& /*index*/)
{
	m_header->setText(tr("Mediafly"));
	m_icon->setVisible(true);

	m_channelModel.cancel();

	m_listView->setItemDelegate(m_itemDelegateDefault);

	m_listView->setModel(NULL);
	m_listView->setModel(&m_menuModel);

	m_listView->setCurrentIndex(m_lastMenuIndex);
	m_listView->setEnabled(true);
	m_listView->setShowLeftArrowIcon(false);
}

void mf::Menu::renderEpisodeMenu(const QModelIndex& index)
{
	m_header->setText(tr("Media Episodes"));
	m_icon->setVisible(false);

	if (index.isValid())
	{
		m_episodeModel.cancel();
		m_episodeModel.clear();

		QString slug = index.data(mf::ChannelModel::slugRole).toString();
		QString name = index.data(mf::ChannelModel::nameRole).toString();

		m_channelSlug = slug;
		m_episodeModel.refresh(mf::EpisodeQuery(slug, 0, itemsReadAtOnce));
	}

	m_listView->setModel(&m_episodeModel);
	m_listView->setItemDelegate(m_itemDelegateEpisode);
	m_listView->setShowLeftArrowIcon(true);
}

void mf::Menu::renderChannelMenu(const QModelIndex& /*index*/)
{
	m_header->setText(m_channelLabel);
	m_icon->setVisible(true);

	m_episodeModel.cancel();
	m_channelModel.refresh();

	m_listView->setModel(&m_channelModel);
	m_listView->setItemDelegate(m_itemDelegateDefault);

	m_listView->setCurrentIndex(m_lastChannelMenuIndex);
	m_listView->setShowLeftArrowIcon(true);
}

void mf::Menu::render(const QModelIndex& index)
{
	switch (m_state) {
	case MainMenu:
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

void mf::Menu::selectMenu(QModelIndex& index)
{
	switch (index.data(mf::MenuModel::slugRole).toInt()) {
	case mf::MenuModel::MENU_SEARCH:
		emit showSearch();
		break;
	case mf::MenuModel::MENU_MEDIA_CHANNELS:
		m_state = ChannelMenu;
		break;
	case mf::MenuModel::MENU_USER:
	{
		// Set correct episode menu's label.

		QString accountName = index.data(mf::MenuModel::nameRole).toString();
		m_channelLabel = accountName + tr("'s Mediafly");

		// If selected user not default already, make him default...

		if (index.data(mf::MenuModel::isDefaultRole).toBool() == false)
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
	case mf::MenuModel::MENU_PERSONALIZE:
		emit showPersonalize();
		break;
	case mf::MenuModel::MENU_REMOVE_PERSON:
	{
		Mediafly::getMediafly()->Authentication_UnbindMFUser(&m_checkResponseOk, m_menuModel.getDefaultAccountName());

		// Remove model to let the MediaflyList render 'Loading menu... Please wait' message.
		// Channel menu will be shown by calling slot showChannelMenu()...

		m_listView->setModel(NULL);
		break;
	}
	default:
		NMessageBox::information(this, tr("Missing feature"), tr("Not yet implemented"));
		break;
	}
}

void mf::Menu::handleEnterKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView->currentIndex();
	if (!index.isValid())
		return;

	switch (m_state) {
	case MainMenu:
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

void mf::Menu::handleRightKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView->currentIndex();
	if (!index.isValid())
		return;

	switch (m_state) {
	case MainMenu:
		m_lastMenuIndex = index;
		selectMenu(index);
		break;
	case ChannelMenu:
		m_lastChannelMenuIndex = index;
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

void mf::Menu::handleLeftKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView->currentIndex();

	switch (m_state) {
	case ChannelMenu:
		m_state = MainMenu;
		break;
	case EpisodeMenu:
		if (m_episodeModel.isSearch())
		{
			m_episodeModel.cancel();
			m_episodeModel.clear();
			emit showSearch();
			return;
		} else
			m_state = ChannelMenu;
		break;
	default:
		return;
	}
	render(index);
}

void mf::Menu::handleExitKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	switch (m_state) {
	case MainMenu:
		QCoreApplication::exit(0);
		break;
	default:
		handleLeftKey();
		break;
	}
}

void mf::Menu::showChannelMenu()
{
	m_state = ChannelMenu;
	render(QModelIndex());
}

void mf::Menu::showEpisodeMenu(SearchQuery& query)
{
	m_episodeModel.refresh(query);
	m_state = EpisodeMenu;
	render(QModelIndex());
}

void mf::Menu::showMainMenu()
{
	m_state = MainMenu;
	render(QModelIndex());
}


