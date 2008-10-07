#include "MediaflyConsumer.h"
#include "MediaflyEpisodeEntry.h"

class MediaflyEpisodeModelData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void entryRead(const MediaflyEpisodeEntry& entry);
	void entryReadFinished();
};

