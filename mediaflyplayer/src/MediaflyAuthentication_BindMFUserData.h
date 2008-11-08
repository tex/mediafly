#ifndef MediaflyAuthentication_BindMFUserData_H
#define MediaflyAuthentication_BindMFUserData_H

#include "MediaflyConsumer.h"

class MediaflyAuthentication_BindMFUserData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void done();
};

#endif
 
