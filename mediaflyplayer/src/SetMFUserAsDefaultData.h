#ifndef mfauthSetMFUserAsDefaultData_H
#define mfauthSetMFUserAsDefaultData_H

#include "Consumer.h"

namespace mf { namespace auth {

class SetMFUserAsDefaultData : public Consumer
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
 
