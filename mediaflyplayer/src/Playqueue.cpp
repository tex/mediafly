#include "Playqueue.h"
#include "MediaflyEpisodeDelegate.h"

using namespace mf;

Playqueue::Playqueue(MediaflyEpisodeModel& episodeModel, QWidget *parent) :
	QWidget(parent),
	m_episodeModel(episodeModel)
{
	setupUi(this);

	m_itemDelegate = new MediaflyEpisodeDelegate(m_listView);

	m_listView->setModel(&m_episodeModel);
	m_listView->setItemDelegate(m_itemDelegate);

	connect(m_listView, SIGNAL(almostAtEndOfList()),
	        this, SLOT(uploadNextPartOfMenu()));
	connect(m_nowPlaying, SIGNAL(clicked()),
	        this, SLOT(handleNowPlayingClicked()));
	connect(m_removeButton, SIGNAL(clicked()),
	        this, SLOT(handleRemoveButtonClicked()));

	connect(&m_episodeModel, SIGNAL(refreshed()),
	        this, SLOT(updateEpisodeModel()));

	connect(&m_checkResponseOk, SIGNAL(done()),
	        this, SLOT(handleCheckResponseOkDone()));
}

void Playqueue::uploadNextPartOfMenu()
{
	qDebug() << __PRETTY_FUNCTION__;

	m_episodeModel.refresh();
}

void Playqueue::updateEpisodeModel()
{
	qDebug() << __PRETTY_FUNCTION__;

	QModelIndex current = m_listView->currentIndex();
	if (!current.isValid())
		current = m_episodeModel.index(0, 0);

	m_listView->setModel(NULL);
	m_listView->setModel(&m_episodeModel);

	m_listView->update(current);
	m_listView->setCurrentIndex(current);
}

void Playqueue::handleNowPlayingClicked()
{
	emit back();
}

void Playqueue::handleRemoveButtonClicked()
{
	QModelIndex current = m_listView->currentIndex();
	if (current.isValid())
	{
		QString slug = current.data(MediaflyEpisodeModel::slugRole).toString();
		Mediafly::getMediafly()->Playlists_RemoveEpisodeFromPlaylist(&m_checkResponseOk, slug);
	}
}

void Playqueue::handleCheckResponseOkDone()
{
	m_episodeModel.refreshFull();
}
