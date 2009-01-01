#ifndef mfConsumer_H
#define mfConsumer_H

#include <QObject>
#include <QDomDocument>

namespace mf {

class Consumer : public QObject
{
public:
	virtual void read(const QDomDocument& doc) = 0;
};

}

#endif

