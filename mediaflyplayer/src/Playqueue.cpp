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

#include "Play.h"
#include "Playqueue.h"
#include "nmessagebox.h"
#include "nhelpbox.h"

using namespace mf;

Playqueue::Playqueue(mf::EpisodeModel& episodeModel, mf::Play *mediaflyPlay, QWidget *parent) :
	NBackgroundManagedWidget(parent),
	m_episodeModel(episodeModel),
	m_mediaflyPlay(mediaflyPlay)
{
	setupUi(this);

	setPreferredBackground(BackgroundVideoOnly);

	m_itemDelegate = new EpisodeDelegate(m_listView);
	m_listView->setModel(&m_episodeModel);
	m_listView->setItemDelegate(m_itemDelegate);

	connect(m_listView, SIGNAL(almostAtEndOfList()),
	        this, SLOT(uploadNextPartOfMenu()));

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
	m_episodeModel.refresh();
}

void Playqueue::updateEpisodeModel()
{
	QModelIndex current = m_listView->currentIndex();
	if (!current.isValid())
		current = m_episodeModel.index(0, 0);

	m_listView->setModel(NULL);
	m_listView->setModel(&m_episodeModel);

	m_listView->update(current);
	m_listView->setCurrentIndex(current);
}

void Playqueue::removeEpisodeFromPlaylist()
{
	QModelIndex current = m_listView->currentIndex();
	if (current.isValid())
	{
		if (NMessageBox::warning(0,
		                     QObject::tr("Remove selected episode from play list?"),
		                     current.data(mf::EpisodeModel::titleRole).toString(),
		                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
		{
			QString slug = current.data(mf::EpisodeModel::slugRole).toString();
			Mediafly::getMediafly()->Playlists_RemoveEpisodeFromPlaylist(&m_checkResponseOk, slug);
		}
	}
}

void Playqueue::handleCheckResponseOkDone()
{
	m_episodeModel.refreshFull();
}

void Playqueue::keyPressEvent(QKeyEvent *event)
{
	// Strong focus is set by designer on Playqueue widget, no focus on
	// anything else on this widget. So, if we detect key event suitable for m_listView
	// we route it to it.

	switch (event->key()) {
	case Qt::Key_Help:
		NHelpBox::NHelpBoxNew(tr("Possible keys"),
		                      tr("Left - Return to previous screen\n") +
		                      tr("Up/Down - Move up/down in menu\n") +
		                      tr("Enter - Remove selected episode from playlist"));
		break;
	case Qt::Key_Up:
	case Qt::Key_Down:
		m_listView->keyPressEvent(event);
		break;
	case Qt::Key_Left:
		emit back();
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return:
		removeEpisodeFromPlaylist();
		break;
	}
}

