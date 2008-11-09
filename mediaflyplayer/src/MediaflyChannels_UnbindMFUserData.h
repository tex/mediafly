#ifndef MediaflyChannels_UnbindMFUserData_H
#define MediaflyChannels_UnbindMFUserData_H

#include "MediaflyConsumer.h"

class MediaflyChannels_UnbindMFUserData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void done();
};

#endif
 
