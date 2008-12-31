#include "BindMFUserData.h"
#include <QDebug>

using namespace mf::auth;

void BindMFUserData::read(const QDomDocument& /*doc*/)
{
	qDebug() << __PRETTY_FUNCTION__;

	emit done();
}

