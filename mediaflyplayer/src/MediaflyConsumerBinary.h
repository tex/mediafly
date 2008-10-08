#ifndef MediaflyConsumerBinary_H
#define MediaflyConsumerBinary_H

#include <QObject>
#include <QByteArray>
#include <QDebug>

class MediaflyConsumerBinary : public QObject
{
	Q_OBJECT
public:
	void read(const QByteArray& array)
	{
		qDebug() << __PRETTY_FUNCTION__;

		emit binaryRead(array);
	}

signals:
	void binaryRead(const QByteArray& array);
};

#endif

