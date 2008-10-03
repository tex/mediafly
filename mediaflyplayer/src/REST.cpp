/************************************************************************
**************************************************************************/

#include "REST.h"
#include <stdlib.h>

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

	Query(Path, useHttps);

	// Block until response read or error signalled.
	//
	m_eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

	if (m_error) {
		throw REST::ConnectionException(m_Http.errorString());
	}

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

void REST::dataDone(int id, bool error)
{
	if (m_id == id) {
		m_error = error;
 		emit singleQueryDone();
	}
}

void REST::Query(QString& Path, bool useHttps)
{
	// Connect signal that is emited from dataDone when
	// http request is finished.
	// 
 	connect(this, SIGNAL(singleQueryDone()), &m_eventLoop, SLOT(quit()));

	// Connect signal that is emited from m_Http when http request
	// is finished.
	//
 	connect(&m_Http, SIGNAL(requestFinished(int,bool)), this, SLOT(dataDone(int, bool)));

	m_Http.setHost(m_Server, useHttps ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp);
 	m_id = m_Http.get(Path);
}

