#ifndef mfauthGetBoundMFUsers_H
#define mfauthGetBoundMFUsers_H

#include "MediaflyConsumer.h"
#include "User.h"

namespace mf { namespace auth {

class GetBoundMFUsersData : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void userRead(const mf::User& entry);
	void userReadFinished();
};

} }

#endif
 
