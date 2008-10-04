#include "BHttp.h"
#include <QEventLoop>

void BHttp::dataDone(int id, bool error)
{
	if (id == m_id)
		emit queryDone();
}

void BHttp::get(const QString& path)
{
	QEventLoop eventLoop;

	connect(this, SIGNAL(requestFinished(int,bool)), this, SLOT(dataDone(int,bool)));
	connect(this, SIGNAL(queryDone()), &eventLoop, SLOT(quit()));

	m_id = QHttp::get(path);

	eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
}

