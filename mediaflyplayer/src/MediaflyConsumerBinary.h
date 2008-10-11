#ifndef MediaflyConsumerBinary_H
#define MediaflyConsumerBinary_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QPixmap>

class MediaflyConsumerBinary : public QObject
{
	Q_OBJECT
public:
	void read(const QString& path, const QByteArray& array)
	{
		qDebug() << __PRETTY_FUNCTION__ << path;

		emit binaryRead(path, array);
	}

signals:
	void binaryRead(const QString& path, const QByteArray& array);
};

#endif

