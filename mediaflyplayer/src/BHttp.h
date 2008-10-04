#ifndef BHttp_h
#define BHttp_h

#include <QHttp>

class BHttp : public QHttp
{
	Q_OBJECT

public:
	void get(const QString& path);

private:
	int m_id;

signals:
	void queryDone();

private slots:
	void dataDone(int id, bool error);
};

#endif

