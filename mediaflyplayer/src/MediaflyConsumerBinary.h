#ifndef MediaflyConsumerBinary_H
#define MediaflyConsumerBinary_H

#include <QObject>
#include <QByteArray>

class MediaflyConsumerBinary : public QObject
{
	Q_OBJECT
public:
	void read(const QByteArray& array)
	{
		emit binaryRead(array);
	}

signals:
	void binaryRead(const QByteArray& array);
};

#endif

