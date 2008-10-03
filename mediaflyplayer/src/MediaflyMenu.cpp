#include "MediaflyMenu.h"
#include <QMessageBox>
#include <QtDebug>
#include <QKeyEvent>

MediaflyMenu::MediaflyMenu(QWidget *parent) :
	QWidget(parent),
	m_listView(this),
	m_state(ChannelMenu)
{
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

	connect(&m_episodeModel, SIGNAL(refreshed()),
	        this, SLOT(updateEpisodeModel()));
	connect(&m_episodeModel, SIGNAL(error(const QString&)),
	        this, SLOT(errorHandler(const QString&)));

	m_channelModel.refresh();
}

void MediaflyMenu::updateChannelModel()
{
	qDebug() << __PRETTY_FUNCTION__;

	m_listView.setModel(NULL);
	m_listView.setModel(&m_channelModel);

	// setModel itself doesn't "refresh", we
	// have to call 'update' to repaint it and
	// 'setCurrentIndex' to select the first item.

	m_listView.update(m_channelModel.index(0, 0));
	m_listView.setCurrentIndex(m_channelModel.index(0, 0));

	m_listView.setEnabled(true);
	m_listView.setFocus();
}

void MediaflyMenu::updateEpisodeModel()
{
	qDebug() << __PRETTY_FUNCTION__;

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
}

void MediaflyMenu::renderEpisodeMenu(const QModelIndex& index)
{
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

void MediaflyMenu::handleEnterKey()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex index = m_listView.currentIndex();

	switch (m_state) {
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

