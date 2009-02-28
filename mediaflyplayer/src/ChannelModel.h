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

#ifndef mfChannelModel_H
#define mfChannelModel_H

#include "Mediafly.h"
#include "ChannelModelData.h"
#include <QAbstractListModel>
#include <QList>

namespace mf {

struct ChannelModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles
	{
		nameRole = Qt::DisplayRole,
		slugRole = Qt::UserRole + 1000,
	};

	ChannelModel(QObject *parent = 0);
	ChannelModel(const ChannelModel &obj);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

	void refresh();
	void cancel();

signals:
	void refreshed();

private:
	Mediafly                   *m_mediafly;
	mf::ChannelModelData        m_modelData;
	QList<mf::ChannelEntry>     m_data;
	int                         m_id;

private slots:
	void handleEntryRead(const mf::ChannelEntry& entry);
	void handleEntryFinished();
};

}

#endif

