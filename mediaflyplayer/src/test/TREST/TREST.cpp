#include "REST.h"

#include "TREST.h"
#include <QtTest>

void TREST::testA()
{
	QString Server("api.mediafly.com");
	QString Prefix("/api/rest/1.1/");
	REST r(Server, Prefix);
	QStringList sl;
//	sl << "app_id=123456" << "token=ABCDEFG";
	QString Method;
//	Method = "Mediafly.Channels.GetChannels";
	qDebug() << r.Query(Method, sl).toString();
	qDebug() << r.Query(Method, sl).toString();
	qDebug() << r.Query(Method, sl).toString();
}
       
// QVERIFY( date.isValid() );
// QCOMPARE( date.month(), 3 );
      
QTEST_MAIN(TREST)

