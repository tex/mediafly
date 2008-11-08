#include "MediaflyAuthentication_BindMFUserData.h"
#include <QDebug>

void MediaflyAuthentication_BindMFUserData::read(const QDomDocument& /*doc*/)
{
	qDebug() << __PRETTY_FUNCTION__;

	emit done();
}

