#ifndef MediaflyCheckResponseOk_H
#define MediaflyCheckResponseOk_H

#include "MediaflyConsumer.h"

class MediaflyCheckResponseOk : public MediaflyConsumer
{
	Q_OBJECT
public:
	void read(const QDomDocument& doc);

signals:
	void done();
};

#endif
 
