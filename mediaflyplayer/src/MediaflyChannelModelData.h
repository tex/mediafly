#ifndef MediaflyChannelModelData_H
#define MediaflyChannelModelData_H

#include "MediaflyConsumer.h"
#include "MediaflyChannelEntry.h"

class MediaflyChannelModelData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void entryRead(const MediaflyChannelEntry& entry);
	void entryReadFinished();
};

#endif

