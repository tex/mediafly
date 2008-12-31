#include "Playqueue.h"

using namespace mf;

Playqueue::Playqueue(mf::EpisodeModel& episodeModel, mf::Play *mediaflyPlay, QWidget *parent) :
	QWidget(parent),
	m_episodeModel(episodeModel),
	m_mediaflyPlay(mediaflyPlay)
{
	setupUi(this);

	m_itemDelegate = new EpisodeDelegate(m_listView);

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

	connect(m_mediaflyPlay, SIGNAL(stateChange()),
		this, SLOT(handleStateChange()));
}

void Playqueue::handleStateChange()
{
	QModelIndex index;
	QString songPosition;
	QString songLength;

	m_mediaflyPlay->getState(index, songPosition, songLength);

	m_songName->setText(index.data(mf::EpisodeModel::titleRole).toString());
	m_songLength->setText(songLength);
	m_songPosition->setText(songPosition);
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
		QString slug = current.data(mf::EpisodeModel::slugRole).toString();
		Mediafly::getMediafly()->Playlists_RemoveEpisodeFromPlaylist(&m_checkResponseOk, slug);
	}
}

void Playqueue::handleCheckResponseOkDone()
{
	m_episodeModel.refreshFull();
}

