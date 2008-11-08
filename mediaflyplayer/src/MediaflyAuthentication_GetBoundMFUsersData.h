#ifndef MediaflyAuthentication_GetBoundMFUsers_H
#define MediaflyAuthentication_GetBoundMFUsers_H

#include "MediaflyConsumer.h"
#include "User.h"

class MediaflyAuthentication_GetBoundMFUsersData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void userRead(const mf::User& entry);
	void userReadFinished();
};

#endif
 
