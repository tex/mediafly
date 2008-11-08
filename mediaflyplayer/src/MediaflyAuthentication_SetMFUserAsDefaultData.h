#ifndef MediaflyAuthentication_SetMFUserAsDefaultData_H
#define MediaflyAuthentication_SetMFUserAsDefaultData_H

#include "MediaflyConsumer.h"

class MediaflyAuthentication_SetMFUserAsDefaultData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& /*doc*/)
	{
		emit ready();
	}

signals:
	void ready();
};

#endif
 
