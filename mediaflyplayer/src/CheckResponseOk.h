#ifndef mfCheckResponseOk_H
#define mfCheckResponseOk_H

#include "Consumer.h"

namespace mf {

class CheckResponseOk : public Consumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void done();
};

}

#endif
 
