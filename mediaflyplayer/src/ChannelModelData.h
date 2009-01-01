#ifndef mfChannelModelData_H
#define mfChannelModelData_H

#include "Consumer.h"
#include "ChannelEntry.h"

namespace mf {

class ChannelModelData : public Consumer
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

