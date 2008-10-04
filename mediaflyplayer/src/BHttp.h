#ifndef BHttp_h
#define BHttp_h

#include <QHttp>

class BHttp : public QHttp
{
	Q_OBJECT

public:
	BHttp() : m_error (false) { }
	bool get(const QString& path);

private:
	int  m_id;
	bool m_error;

signals:
	void queryDone();

private slots:
	void dataDone(int id, bool error);
};

#endif

