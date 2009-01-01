#ifndef mfEpisodeModelData_H
#define mfEpisodeModelData_H

#include "Consumer.h"
#include "EpisodeEntry.h"
#include <QDebug>

namespace mf {

class EpisodeModelData : public Consumer
{
	Q_OBJECT
public:
	EpisodeModelData() : m_totalEpisodes(-1) { }

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

}

#endif

