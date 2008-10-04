/************************************************************************
**************************************************************************/

#ifndef REST_H
#define REST_H

#include <QString>
#include <QStringList>
#include <QDomDocument>
#include "BHttp.h"
#include "Exception.h"

/**
  * class REST
  * Primitive implementation of the REST protocol. Query() sends an GET request to
  * the server and returns an response as instance of QDomDocument class.
  */

class REST : public QObject
{
	Q_OBJECT
public:
	class ParseException : public Exception
	{
	public:
		ParseException(QString errorMsg, int errorLine, int errorColumn, QString document) :
			Exception(errorMsg)
		{ }
	};
	class ConnectionException : public Exception
	{
	public:
		ConnectionException(QString errorMsg) :
			Exception(errorMsg)
		{ }
	};

	/**
	 * Constructs the path and sends the GET reguest to the server.
	 * @return QDomDocument
	 * @param  Parameters Parameters from wich a patch is assembled.
	 * @param  useHttps If true, uses https. Otherwise use http. Https is required for
	 * Mediafly.Authentication.BindMFUser.
	 */
	QDomDocument Query (QString Method, QStringList& Parameters, bool useHttps = false);

	/**
	 * Constructor.
	 * @param  Server Address of the REST server on the Internet. Like
	 * 'http://api.mediafly.com/'.
	 */
	REST (QString Server, QString Prefix);

	virtual ~REST();

private:
	QString  m_Server;
	QString  m_Prefix;
	BHttp    m_Http;
};

#endif // REST_H
