#ifndef MediaflyEpisodeModel_H
#define MediaflyEpisodeModel_H

#include <QAbstractListModel>
#include <QStringList>
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
		channelRole,
	};

	MediaflyEpisodeModel(QObject *parent = 0); 
	MediaflyEpisodeModel(const MediaflyEpisodeModel &obj);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

	void refresh(QString channelSlug, int offset, int limit, QString mediaType = "audio,video" );
	void unite(const MediaflyEpisodeModel &obj);

	QString toString() const ;
private:

	// 1. Key : offset
	// 1. Value : 2. Key : role
	//            2. Value: value

	QMap<int, QMap<int, QString> > m_data;
};

#endif

