#ifndef MediaflyConsumer_H
#define MediaflyConsumer_H

#include <QObject>
#include <QDomDocument>

class MediaflyConsumer : public QObject
{
public:
	virtual void read(const QDomDocument& doc) = 0;
};

#endif

