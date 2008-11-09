#include "MediaflyChannels_UnbindMFUserData.h"
#include <QDebug>

void MediaflyChannels_UnbindMFUserData::read(const QDomDocument& /*doc*/)
{
	qDebug() << __PRETTY_FUNCTION__;

	emit done();
}

