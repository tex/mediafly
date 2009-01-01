#include "CheckResponseOk.h"
#include <QDebug>

void mf::CheckResponseOk::read(const QDomDocument& /*doc*/)
{
	qDebug() << __PRETTY_FUNCTION__;

	emit done();
}

