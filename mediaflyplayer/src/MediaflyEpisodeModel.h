#ifndef MediaflyEpisodeModel_H
#define MediaflyEpisodeModel_H

#include "MediaflyEpisodeModelThread.h"
#include "MediaflyEpisodeEntry.h"
#include <QAbstractListModel>
#include <QMap>
#include <QPixmap>

class MediaflyEpisodeModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles
	{
		titleRole = Qt::DisplayRole,
		slugRole = Qt::UserRole,
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

	MediaflyEpisodeModel(QObject *parent = 0); 
	MediaflyEpisodeModel(const MediaflyEpisodeModel &obj);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

	void clear();

public slots:
	void refresh(QString channelSlug, int offset, int limit, QString mediaType = "audio,video");

signals:
	void entryRefreshed();
	void imageRefreshed();
	void error(const QString& errorMsg);

private:

	// 1. Key : offset
	// 1. Value : 2. Key : role
	//            2. Value: value

	QMap<int, MediaflyEpisodeEntry> m_data;
	QMap<int, QByteArray> m_image;

	MediaflyEpisodeModelThread m_episodeModelThread;

private slots:
	void handleError(const QString& errorMsg);
	void handleEntry(const MediaflyEpisodeEntry& entry);
	void handleImage(const QByteArray& buffer);
};

#endif

