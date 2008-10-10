#ifndef MediaflyEpisodeModel_H
#define MediaflyEpisodeModel_H

#include "Mediafly.h"
#include "MediaflyEpisodeEntry.h"
#include "MediaflyEpisodeQuery.h"
#include "MediaflyConsumerBinary.h"
#include <QAbstractListModel>
#include <QMap>

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

	void refresh(const MediaflyEpisodeQuery& query);
	void refresh();
	void cancel();

signals:
	void refreshed();

private:
	Mediafly                       *m_mediafly;
	MediaflyEpisodeModelData        m_modelData;
	MediaflyConsumerBinary          m_binaryData;
	QMap<int, MediaflyEpisodeEntry> m_data;
	QMap<int, QByteArray>           m_image;
	bool                            m_refreshFinished;
	MediaflyEpisodeQuery            m_query;

private slots:
	void handleEntryRead(const MediaflyEpisodeEntry& entry);
	void handleEntryReadFinished();
	void handleBinaryRead(const QByteArray& buffer);
};

#endif

