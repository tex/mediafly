#ifndef mfCheckResponseOk_H
#define mfCheckResponseOk_H

#include "MediaflyConsumer.h"

namespace mf {

class CheckResponseOk : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void done();
};

}

#endif
 
