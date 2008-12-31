#include "MediaflyConsumer.h"
#include "EpisodeEntry.h"
#include <QDebug>

class MediaflyEpisodeModelData : public MediaflyConsumer
{
	Q_OBJECT
public:
	MediaflyEpisodeModelData() : m_totalEpisodes(-1) { }
	void read(const QDomDocument& doc);

	/**
	 * Returns total number of all episodes available.
	 * Return value valid after emmited entryRead() or entryReadFinished()
	 * signal.
	 */
	int totalEpisodes() const { return m_totalEpisodes; }

	void clear() { m_totalEpisodes = -1; }

signals:
	void entryRead(const mf::EpisodeEntry& entry);
	void entryReadFinished();

private:
	int m_totalEpisodes;
};

