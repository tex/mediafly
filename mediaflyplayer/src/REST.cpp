/************************************************************************
**************************************************************************/

#include "REST.h"
#include <stdlib.h>
#include <QDebug>
#include <QUrl>

/**
 * Constructs the path and sends the GET reguest to the server.
 * @return QDomDocument
 * @param  Parameters Parameters from wich a patch is assembled.
 * @param  useHttps If true, uses https. Otherwise use http. Https is required for
 * Mediafly.Authentication.BindMFUser.
 */
QDomDocument REST::Query (QString Method, QStringList& Parameters, bool useHttps )
{
	qDebug() << Method;

	QString Path = m_Prefix + Method;
	if (Parameters.size() > 0)
		Path += "?";
	for (int i = 0; i < Parameters.size(); ++i) {
		Path += Parameters.at(i);
		if (i + 1 != Parameters.size())
			Path += "&";
	}

	m_Http.setHost(m_Server, useHttps ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp);
 	if (m_Http.get(Path) == false)
		throw REST::ConnectionException(m_Http.errorString());

	// Make an DOM document from the response.
	// 
	QDomDocument dom;
	QString document = QString::fromUtf8(m_Http.readAll());

	QString errorMsg;
	int errorLine;
	int errorColumn;
	if (!dom.setContent(document, &errorMsg, &errorLine, &errorColumn)) {
		throw REST::ParseException(errorMsg, errorLine, errorColumn, document);
	}
	return dom;
}


/**
 * Constructor.
 * @param  Server Address of the REST server on the Internet. Like
 * 'http://api.mediafly.com/'.
 */
REST::REST (QString Server, QString Prefix ) : m_Server(Server),
                                               m_Prefix(Prefix)
{
	QUrl proxy(getenv("http_proxy"));
	m_Http.setProxy(proxy.host(), proxy.port());
}

REST::~REST() {
}


