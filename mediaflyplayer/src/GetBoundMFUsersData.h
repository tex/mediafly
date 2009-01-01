#ifndef mfauthGetBoundMFUsers_H
#define mfauthGetBoundMFUsers_H

#include "Consumer.h"
#include "User.h"

namespace mf { namespace auth {

class GetBoundMFUsersData : public Consumer
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
 
