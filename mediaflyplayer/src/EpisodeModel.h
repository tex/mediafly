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

#ifndef mfEpisodeModel_H
#define mfEpisodeModel_H

#include "Mediafly.h"
#include "EpisodeEntry.h"
#include "EpisodeQuery.h"
#include "ConsumerBinary.h"
#include <QAbstractListModel>
#include <QMap>
#include <QString>
#include <QByteArray>
#include <QPixmap>

namespace mf {

class EpisodeModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles
	{
		titleRole = Qt::DisplayRole,
		slugRole = Qt::UserRole + 1000,
		descriptionRole,
		formatRole,
		urlRole,
		urlOriginalRole,
		publishedRole,
		showSlugRole,
		showTitleRole,
		imageUrlRole,
		imageRole,
		channelRole,
	};

	EpisodeModel(QObject *parent = 0); 
	EpisodeModel(const EpisodeModel &obj);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

	void clear();

	void refresh(const mf::EpisodeQuery& query);
	void refresh();
	void refreshFull();
	void cancel();

	int totalRowCount() const;

	static bool advanceToNextEpisode(QModelIndex& index)
	{
		if (index.row() + 15 > index.model()->rowCount())
			dynamic_cast<EpisodeModel *>(
				const_cast<QAbstractItemModel *>(index.model())
			)->refresh();
		if (index.row() + 1 < index.model()->rowCount()) {
			index = index.model()->index(index.row() + 1, 0);
			return true;
		}
		return false;
	}

	static bool advanceToPreviousEpisode(QModelIndex& index)
	{
		if (index.row() > 0) {
			index = index.model()->index(index.row() - 1, 0);
			return true;
		}
		return false;
	}

signals:
	void refreshed();

private:
	Mediafly                       *m_mediafly;
	mf::EpisodeModelData            m_modelData;
	mf::ConsumerBinary              m_binaryData;
	QMap<int, mf::EpisodeEntry>     m_data;
	bool                            m_refreshFinished;
	mf::EpisodeQuery                m_query;

private slots:
	void handleEntryRead(const mf::EpisodeEntry& entry);
	void handleEntryReadFinished();
	void handleBinaryRead(const QString& path, const QByteArray& array);
};

}

#endif

