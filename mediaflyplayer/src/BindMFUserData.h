#ifndef mfauthBindMFUserData_H
#define mfauthBindMFUserData_H

#include "MediaflyConsumer.h"

namespace mf { namespace auth {

class BindMFUserData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void done();
};

} }

#endif

