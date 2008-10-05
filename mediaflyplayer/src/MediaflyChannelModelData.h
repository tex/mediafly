#include "MediaflyChannelEntry.h"
#include <QObject>

class MediaflyChannelModelData : public QObject
{
	Q_OBJECT
public:
	MediaflyChannelModelData(QObject *parent = 0) :
		QObject(parent)
	{ }

public slots:
	// Reads all available channel entries. When new
	// entry loaded, emits dataCompleted() signal.
	//
	void readData();

signals:
	void entryRead(const MediaflyChannelEntry& entry);
};

