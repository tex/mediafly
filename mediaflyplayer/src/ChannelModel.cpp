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
	qDebug() << __PRETTY_FUNCTION__;

	for (int i = m_id; i < m_data.size(); ++i) {
		m_data.removeAt(i);
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

