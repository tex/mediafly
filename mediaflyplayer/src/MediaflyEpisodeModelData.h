#include "MediaflyEpisodeEntry.h"
#include <QObject>
#include <QString>

class MediaflyEpisodeModelData : public QObject
{
	Q_OBJECT
public:
	MediaflyEpisodeModelData(QObject *parent = 0) :
		QObject(parent)
	{ }

	QByteArray readImage(const QString& imageUrl);
	void readData(QString channelSlug, int offset, int limit, QString mediaType = "audio,video");

signals:
	void entryRead(const MediaflyEpisodeEntry& entry);
	void imageRead(const QString& imageUrl);
};

