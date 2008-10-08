#include "MediaflyConsumer.h"
#include "MediaflyEpisodeEntry.h"

class MediaflyEpisodeModelData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

	/**
	 * Returns total number of all episodes available.
	 * Return value valid after emmited entryRead() or entryReadFinished()
	 * signal.
	 */
	int totalEpisodes() const { return m_totalEpisodes; }

signals:
	void entryRead(const MediaflyEpisodeEntry& entry);
	void entryReadFinished();

private:
	int m_totalEpisodes;
};

