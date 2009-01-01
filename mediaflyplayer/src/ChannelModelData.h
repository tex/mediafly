#ifndef mfChannelModelData_H
#define mfChannelModelData_H

#include "MediaflyConsumer.h"
#include "ChannelEntry.h"

namespace mf {

class ChannelModelData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void entryRead(const mf::ChannelEntry& entry);
	void entryReadFinished();
};

}

#endif

