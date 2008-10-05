#include "MediaflyEpisodeModel.h"
#include <QDebug>

MediaflyEpisodeModel::MediaflyEpisodeModel(QObject *parent) :
	QAbstractListModel(parent)
{
	qRegisterMetaType<MediaflyEpisodeModel>("MediaflyEpisodeModel");

	connect(&m_episodeModelThread, SIGNAL(entryRead(const MediaflyEpisodeEntry&)),
	        this, SLOT(handleEntry(const MediaflyEpisodeEntry&)));
	connect(&m_episodeModelThread, SIGNAL(imageRead(const QByteArray&)),
	        this, SLOT(handleImage(const QByteArray&)));
	connect(&m_episodeModelThread, SIGNAL(error(const QString&)),
	        this, SLOT(handleError(const QString&)));
}

MediaflyEpisodeModel::MediaflyEpisodeModel(const MediaflyEpisodeModel& obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_data = obj.m_data;
	m_image = obj.m_image;
}

void MediaflyEpisodeModel::clear()
{
	m_data.clear();
	m_image.clear();
}

void MediaflyEpisodeModel::refresh(QString channelSlug, int offset, int limit, QString mediaType)
{
	m_episodeModelThread.refresh(channelSlug, offset, limit, mediaType);
}

void MediaflyEpisodeModel::handleError(const QString& errorMsg)
{
	emit error(errorMsg);
}

void MediaflyEpisodeModel::handleEntry(const MediaflyEpisodeEntry& entry)
{
	m_data[m_data.size()] = entry;
	emit entryRefreshed();
}

void MediaflyEpisodeModel::handleImage(const QByteArray& buffer)
{
	qDebug() << "image added to the model";
	m_image[m_image.size()] = buffer;
	emit imageRefreshed();
}

int MediaflyEpisodeModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_data.size();
}

QVariant MediaflyEpisodeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_data.size())
		return QVariant();

	switch (role) {
	case titleRole:       return (m_data[index.row()]).title();
	case slugRole:        return (m_data[index.row()]).slug();
	case descriptionRole: return (m_data[index.row()]).description();
	case formatRole:      return (m_data[index.row()]).format();
	case urlRole:         return (m_data[index.row()]).url();
	case urlOriginalRole: return (m_data[index.row()]).urlOriginal();
	case publishedRole:   return (m_data[index.row()]).published();
	case showSlugRole:    return (m_data[index.row()]).showSlug();
	case showTitleRole:   return (m_data[index.row()]).showTitle();
	case imageUrlRole:    return (m_data[index.row()]).imageUrl();
	case imageRole:       return (m_image[index.row()]);
	case channelRole:     return (m_data[index.row()]).channel();
	default:              return QVariant();
	}
}

