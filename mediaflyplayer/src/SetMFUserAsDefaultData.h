#ifndef mfauthSetMFUserAsDefaultData_H
#define mfauthSetMFUserAsDefaultData_H

#include "MediaflyConsumer.h"

namespace mf { namespace auth {

class SetMFUserAsDefaultData : public MediaflyConsumer
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

} }

#endif
 
