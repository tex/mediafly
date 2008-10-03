#include "MediaflyMenu.h"
#include <QMessageBox>
#include <QtDebug>
#include <QKeyEvent>

MediaflyMenu::MediaflyMenu(QWidget *parent) :
	QWidget(parent),
	m_listView(this),
	m_state(ChannelMenu),
	m_channelModel(NULL),
	m_episodeModel(NULL)
{
	m_layout.addWidget(&m_listView);
	setLayout(&m_layout);

	connect(&m_listView, SIGNAL(almostAtEndOfList()),
	        this, SLOT(uploadNextPartOfMenu()));
	connect(&m_listView, SIGNAL(enterPressed()),
	        this, SLOT(handleEnterKey()));
	connect(&m_listView, SIGNAL(rightPressed()),
	        this, SLOT(handleRightKey()));
	connect(&m_listView, SIGNAL(leftPressed()),
	        this, SLOT(handleLeftKey()));

	connect(&m_channelModelThread, SIGNAL(refreshed(const MediaflyChannelModel&)),
	        this, SLOT(updateModel(const MediaflyChannelModel&)));
	connect(&m_channelModelThread, SIGNAL(error(const QString&)),
	        this, SLOT(errorHandler(const QString&)));
	connect(&m_episodeModelThread, SIGNAL(refreshed(const MediaflyEpisodeModel&)),
	        this, SLOT(updateModel(const MediaflyEpisodeModel&)));
	connect(&m_episodeModelThread, SIGNAL(error(const QString&)),
	        this, SLOT(errorHandler(const QString&)));
	m_channelModelThread.refresh();
}

void MediaflyMenu::updateModel(const MediaflyChannelModel& m)
{
	delete m_channelModel;
	m_channelModel = new MediaflyChannelModel(m);

	m_listView.setModel(m_channelModel);

	// setModel itself doesn't "refresh", we
	// have to call 'update' to repaint it and
	// 'setCurrentIndex' to select the first item.

	m_listView.update(m_channelModel->index(0, 0));
	m_listView.setCurrentIndex(m_channelModel->index(0, 0));

	// Clear MediaflyEpisodeModel model.

	delete m_episodeModel;
	m_episodeModel = NULL;

	m_listView.setEnabled(true);
	m_listView.setFocus();
}

void MediaflyMenu::updateModel(const MediaflyEpisodeModel& m)
{
	bool newModel = false;

	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex current = m_listView.currentIndex();

	if (!m_episodeModel) {
		m_episodeModel = new MediaflyEpisodeModel();
		newModel = true;
	}

	qDebug() << "Merge";
	qDebug() << m_episodeModel->toString();
	qDebug() << "with:";
	qDebug() << m.toString();

	m_episodeModel->unite(m);
	m_listView.setModel(NULL);
	m_listView.setModel(m_episodeModel);

	// setModel itself doesn't "refresh", we
	// have to call 'update' to repaint it and
	// 'setCurrentIndex' to select the first item.

	if (newModel)
		current = m_episodeModel->index(0,0);

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
	m_episodeModelThread.refresh(slug, 0, itemsReadAtOnce);

	m_state = EpisodeMenu;
	m_listView.setEnabled(false);
}

void MediaflyMenu::renderChannelMenu(const QModelIndex& /*index*/)
{
	m_channelModelThread.refresh();

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
		if (!m_episodeModel)
			return;
		m_episodeModelThread.refresh(m_channelSlug, m_episodeModel->rowCount(), itemsReadAtOnce);
		return;
	default:
		return;
	}
}

