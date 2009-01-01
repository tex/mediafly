#ifndef mfConsumerBinary_H
#define mfConsumerBinary_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QPixmap>

namespace mf {

class ConsumerBinary : public QObject
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

}

#endif

