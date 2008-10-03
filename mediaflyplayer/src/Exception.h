#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

class Exception {
	QString m_errorMsg;
public:
	Exception(QString errorMsg) : m_errorMsg (errorMsg) { }
	QString getError() const { return m_errorMsg; }
};

#endif

