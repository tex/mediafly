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

#include "ChannelModel.h"
#include <QDebug>

using namespace mf;

ChannelModel::ChannelModel(QObject *parent) :
	QAbstractListModel(parent)
{
	m_mediafly = Mediafly::getMediafly();

	connect(&m_modelData, SIGNAL(entryRead(const mf::ChannelEntry&)),
	        this, SLOT(handleEntryRead(const mf::ChannelEntry&)));

	connect(&m_modelData, SIGNAL(entryReadFinished()),
	        this, SLOT(handleEntryFinished()));
}

ChannelModel::ChannelModel(const ChannelModel &obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_mediafly = Mediafly::getMediafly();
	m_data = obj.m_data;
}

void ChannelModel::cancel()
{
	// TODO
}
void ChannelModel::refresh()
{
	m_id = 0;
	m_mediafly->Channels_GetChannels(&m_modelData, true);
}

void ChannelModel::handleEntryRead(const mf::ChannelEntry& entry)
{
	qDebug() << __PRETTY_FUNCTION__ << "id:" << m_id << entry.name();

	if (m_data.size() > m_id)
		m_data.replace(m_id, entry);
	else
		m_data.insert(m_id, entry);
	++m_id;

	emit refreshed();
}

void ChannelModel::handleEntryFinished()
{
	int size = m_data.size();

	for (int i = m_id; i < size; ++i) {
		m_data.removeLast();
	}

	emit refreshed();
}

int ChannelModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_data.size();
}

QVariant ChannelModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_data.size())
		return QVariant();

	switch (role) {
	case nameRole: return m_data.at(index.row()).name();
	case slugRole: return m_data.at(index.row()).slug();
	default:       return QVariant();
	}
}

