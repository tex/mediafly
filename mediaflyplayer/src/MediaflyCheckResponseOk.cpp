#include "MediaflyCheckResponseOk.h"
#include <QDebug>

void MediaflyCheckResponseOk::read(const QDomDocument& /*doc*/)
{
	qDebug() << __PRETTY_FUNCTION__;

	emit done();
}

