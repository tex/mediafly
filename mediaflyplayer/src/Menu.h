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

#ifndef mfMenu_H
#define mfMenu_H

#include "MenuModel.h"
#include "ChannelModel.h"
#include "EpisodeModel.h"
#include "EpisodeDelegate.h"
#include "CheckResponseOk.h"
#include "ui_MediaflyMenu.h"
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace mf {

class Menu : public QWidget, protected Ui::MediaflyMenu
{
	Q_OBJECT
public:
	Menu(mf::MenuModel&         menuModel,
	     mf::ChannelModel&      channelModel,
	     mf::EpisodeModel&      episodeModel,
	     QWidget               *parent = 0); 

	void showChannelsMenu();

signals:
	void showSearch();
	void showPlayMenu(const QModelIndex& index);
	void showShowMenu(const QModelIndex& index);
	void showPersonalize();
	void showLoginPerson();

public slots:
	void showChannelMenu();

private:
	enum State
	{
		MainMenu,
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
	mf::CheckResponseOk                           m_checkResponseOk;

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

}

#endif

