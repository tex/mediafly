#ifndef mfauthBindMFUserData_H
#define mfauthBindMFUserData_H

#include "Consumer.h"

namespace mf { namespace auth {

class BindMFUserData : public Consumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void done();
};

} }

#endif

