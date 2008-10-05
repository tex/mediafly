#include "MediaflyChannelModel.h"

MediaflyChannelModel::MediaflyChannelModel(QObject *parent) :
	QAbstractListModel(parent)
{
	connect(&m_channelModelThread, SIGNAL(entryRead(const MediaflyChannelEntry&)),
	        this, SLOT(handleEntry(const MediaflyChannelEntry&)));
	connect(&m_channelModelThread, SIGNAL(error(const QString&)),
	        this, SLOT(handleError(const QString&)));
}

MediaflyChannelModel::MediaflyChannelModel(const MediaflyChannelModel &obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_data = obj.m_data;
}

void MediaflyChannelModel::refresh()
{
	m_data.clear();
	m_channelModelThread.refresh();
}

void MediaflyChannelModel::handleError(const QString& errorMsg)
{
	emit error(errorMsg);
}

void MediaflyChannelModel::handleEntry(const MediaflyChannelEntry& entry)
{
	m_data << entry;
	emit refreshed();
}

int MediaflyChannelModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_data.size();
}

QVariant MediaflyChannelModel::data(const QModelIndex &index, int role) const
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

