/*
 *  Copyright(C) 2009 Milan Svoboda. 
 *               <milan.svoboda@centrum.cz>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *  
 *
 *  This program is distributed in the hope that, in addition to its 
 *  original purpose to support Neuros hardware, it will be useful 
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************/

#include "Mediafly.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QNetworkInterface>
#include <QUrl>
#include <QFile>

#include <stdlib.h>

using namespace mf;

extern QString  currentPath;

Mediafly*       Mediafly::m_mediafly = NULL;
const QString   Mediafly::m_server = "api.mediafly.com";
const QString   Mediafly::m_prefix = "/api/rest/1.1/Mediafly.";

Mediafly* Mediafly::getMediafly()
{
	if (!m_mediafly) {
		m_mediafly = new Mediafly();
	}
	return m_mediafly;
}

/**
 * Check request response. Is it correct and is it's status fail or ok?
 * 
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="fail">
 *   <err code="14" message="Request must be made over a secure connection." />
 * </response>
 */
bool Mediafly::checkResponse(QDomDocument& doc, QString& data, QString& errorMsg, bool& expiredToken)
{
	expiredToken = false;

	if (!doc.setContent(data)) {
		errorMsg = data;
		return false;
	}

	QDomElement docResponse = doc.firstChildElement("response");
	if (docResponse.isNull()) {
		errorMsg = data;
		return false;
	}

	QString statusAttribute = docResponse.attribute("status");
	if (statusAttribute == "fail") {
		QDomElement docErr = docResponse.firstChildElement("err");
		QString errCode = docErr.attribute("code");
		QString errMessage = docErr.attribute("message");
		if ((errCode.toInt() == -1) && (errMessage == "Token invalid."))
			expiredToken = true;
		errorMsg = "[" + errCode + "] " + errMessage;
		return false;
	} else if (statusAttribute != "ok") {
		errorMsg = doc.toString();
		return false;
	}
	return true;
}

void Mediafly::handleRequestFinished(int id, bool error)
{
	if (m_connection.contains(id))
	{
		RequestInfo requestInfo = m_connection.value(id);

		if (error) {
			emit readError(m_http.errorString());
		}
		else {
			QString data = QString::fromUtf8(m_http.readAll());
			QString errorMsg;
			QDomDocument doc;

			qDebug() << data;

			bool expiredToken;
			if (!checkResponse(doc, data, errorMsg, expiredToken))
			{
				if (expiredToken)
				{
					// Acquire new session info and restart
					// the operation.

					Authentication_GetToken();
					m_request << requestInfo;
				}
				else
					emit readError(errorMsg);
			}
			else
				requestInfo.m_consumer->read(doc);
		}

		m_connection.remove(id);

		if (m_request.size() > 0) {
			RequestInfo requestInfo = m_request.takeFirst();
			Query(requestInfo);
		}
	}
	if (m_connectionBinary.contains(id))
	{
		RequestInfoBinary requestInfoBinary = m_connectionBinary.take(id);
		QByteArray array = m_http.readAll();
		requestInfoBinary.m_consumer->read(requestInfoBinary.m_path, array);
	}
}

Mediafly::Mediafly()
{
	QFile config(currentPath + "/.mediafly.conf");
	if (config.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		m_appId = config.readLine().trimmed();
		m_sharedSecret = config.readLine().trimmed();
	}

	// Get the third party user id from hardware address of the first
	// network non-loop interface.

	foreach (QNetworkInterface networkInterface, QNetworkInterface::allInterfaces())
		if ((m_thirdPartyUserId = networkInterface.hardwareAddress()) != "00:00:00:00:00:00")
			break;

	qDebug() << "m_appId:" << m_appId << ", m_sharedSecret:" << m_sharedSecret << ", m_thirdPartyUserId:" << m_thirdPartyUserId;

	connect(&m_http, SIGNAL(requestFinished(int, bool)),
	        this, SLOT(handleRequestFinished(int, bool)));

	// Get token as soon as possible. It will be neccessary
	// anyway...

	Authentication_GetToken();
}

void Mediafly::abort()
{
	m_connection.clear();
	m_connectionBinary.clear();
	m_http.abort();
}

QStringList Mediafly::makeParams(QMap<QString, QString>& map)
{
	QStringList sl;
	for (QMap<QString, QString>::const_iterator it = map.constBegin();
	     it != map.constEnd(); ++it) {
		sl << (it.key() + QString("=") + it.value());
	}
	return sl;
}

QString Mediafly::makePath(QString& method, QStringList& parameters)
{
	QString path = m_prefix + method;
	if (parameters.size() > 0)
		path += "?";
	for (int i = 0; i < parameters.size(); ++i) {
		path += parameters.at(i);
		if (i + 1 != parameters.size())
			path += "&";
	}
	qDebug() << path;
	return path;
}

void Mediafly::Query(RequestInfoBinary& requestInfoBinary)
{
	// Ignore duplicate queries....

	foreach (RequestInfoBinary tmp, m_connectionBinary) {
		if (tmp.m_path == requestInfoBinary.m_path)
			return;
	}
#ifndef QT_NO_NETWORKPROXY
	// Dunno why this doesn't work when in the constructor...

	QUrl proxy(getenv("http_proxy"));
	m_http.setProxy(proxy.host(), proxy.port());
#endif
	QUrl url(requestInfoBinary.m_path);

	m_http.setHost(url.host(), QHttp::ConnectionModeHttp);
 	int id = m_http.get(url.path());

	m_connectionBinary.insert(id, requestInfoBinary);
}

void Mediafly::Query (mf::ConsumerBinary *consumer, const QString& path)
{
	RequestInfoBinary requestInfoBinary;
	requestInfoBinary.m_consumer = consumer;
	requestInfoBinary.m_path = path;

	Query(requestInfoBinary);
}

void Mediafly::Query (RequestInfo& requestInfo)
{
#ifndef QT_NO_NETWORKPROXY
	// Dunno why this doesn't work when in the constructor...

	QUrl proxy(getenv("http_proxy"));
	m_http.setProxy(proxy.host(), proxy.port());
#endif
	m_http.setHost(m_server, requestInfo.m_useHttps ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp);

	QStringList ls = makeParams(requestInfo.m_firstMap) + makeParams(requestInfo.m_map);

 	int id = m_http.get(makePath(requestInfo.m_method, ls));
	m_connection.insert(id, requestInfo);
}

void Mediafly::Query (mf::Consumer *consumer, QString method, QMap<QString, QString>& firstMap, QMap<QString, QString>& map, bool useHttps )
{
	qDebug() << method;

	RequestInfo requestInfo;
	requestInfo.m_consumer = consumer;
	requestInfo.m_method = method;
	requestInfo.m_firstMap = firstMap;
	requestInfo.m_map = map;
	requestInfo.m_useHttps = useHttps;

	if (m_connection.size() == 0) {
		Query(requestInfo);
	} else {
		m_request << requestInfo;
	}
}

void Mediafly::Query(mf::Consumer *consumer, QString function, QMap<QString, QString>& map)
{
	QMap<QString, QString> firstMap;
	firstMap[QString("app_id")] = m_appId;

	Query(consumer, function, firstMap, map);
}

QString Mediafly::computeHash(QMap<QString, QString>& map, QString tokenId)
{
	QCryptographicHash hash(QCryptographicHash::Md5);

	hash.addData(m_sharedSecret.toUtf8());
	hash.addData(tokenId.toUtf8());

	for (QMap<QString, QString>::const_iterator it = map.constBegin();
	     it != map.constEnd(); ++it) {
		hash.addData(it.value().toUtf8());
	}

	return QString(hash.result().toHex());
}

void Mediafly::Query(mf::Consumer *consumer, QString function, QMap<QString, QString>& map, const mf::SessionInfo& session, bool useHttps)
{
	QMap<QString, QString> firstMap;
	firstMap[QString("app_id")] = m_appId;
	firstMap[QString("token")] = session.token();
	firstMap[QString("call_sig")] = computeHash(map, session.tokenId());

	Query(consumer, function, firstMap, map, useHttps);
}

void Mediafly::read(const QDomDocument& doc)
{
	QDomElement docToken = doc.firstChildElement("response").firstChildElement("token");
	if (docToken.isNull()) {
		emit readError(doc.toString());
	}

	m_sessionInfo.setToken(docToken.text());
	m_sessionInfo.setTokenId(docToken.attribute("id"));

	// Update token on all queued connections.

	for (int i = 0; i < m_request.size(); ++i) {
		RequestInfo requestInfo = m_request.at(i);
		requestInfo.m_firstMap[QString("token")] = m_sessionInfo.token();
		requestInfo.m_firstMap[QString("call_sig")] = computeHash(requestInfo.m_map, m_sessionInfo.tokenId());
		m_request.replace(i, requestInfo);
	}
}

/**
 * This method retrieves a image (raw data) from specified url.
 */
void Mediafly::Utility_GetImage(mf::ConsumerBinary *consumer, const QString& path)
{
	Query(consumer, path);
}

/**
 * This method retrieves a session token from the server.  This method returns a
 * token to be used in all subsequent API calls.  The token is good for 60 minutes
 * from time of issue.
 * Response:
 * <response status="ok">
 * <token id="6fb071b069c545d6b7c60b5cc6ec82be">TOKEN</token>
 * </response>
 * @param  thirdPartyUserID a unique id that will be used to uniquely identify the
 * device, or the user using the device or application
 */
void Mediafly::Authentication_GetToken ()
{
	QMap<QString, QString> map;
	map["thirdPartyUserID"] = m_thirdPartyUserId;
	Query(this, "Authentication.GetToken", map);
}

/**
 * This method returns a list of channels.
 * Response:
 * <response status="ok">
 *   <channels>
 *     <channel name="All (Mix)" slug="__all__" />
 *     <channel name="business" slug="business" />
 *     <channel name="education" slug="education" />
 *   </channels>
 * </response>
 * Response (No Channels):
 * <response status="ok">
 * 
 *   <channels />
 * </response>
 * @param  capitalizeChannelNames (optional):  whether channel names will be
 * capitalized (defaults to “true”, excepts values “true” or “false”)
 */
void Mediafly::Channels_GetChannels (mf::ChannelModelData *modelData,
                                     bool capitalizeChannelNames)
{
	QMap<QString, QString> map;
	map["capitalizeChannelNames"] = capitalizeChannelNames ? "true" : "false";
	Query(modelData, QString("Channels.GetChannels"), map, m_sessionInfo);
}

/**
 * This method returns a list of episodes for the specified channel.
 * Response:
 * <response status="ok">
 *   <playlist channelSlug="business" totalEpisodes="3">
 *     <episode slug=““ title="" description=”” format="" url="" urlOriginal=""
 * published=””
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *     <episode slug=““ title="" description=”” format="" url="" urlOriginal=""
 * published=””
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *     <episode slug=““ title="" description=”” format="" url="" urlOriginal=""
 * published=””
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </playlist>
 * </response>
 * Response (No Episodes):
 * <response status="ok">
 *   <playlist channelSlug="business" />
 * </response>
 * @param  channelSlug  (required):  the slug for the requested channel
 * @param  offset (optional):  an integer value representing the offset for the
 * start of the playlist results (default: 0)
 * @param  limit (optional): an integer value representing the number of episodes
 * to return (default:500, max:500)
 * @param  mediaType (optional):  include or exclude content based on its type
 * (“audio”, “video”, default: “audio,video”)
 */
void Mediafly::Playlists_GetPlaylistForChannel (mf::EpisodeModelData* modelData,
                                                const mf::EpisodeQuery& query)
{
	QMap<QString, QString> map;
	map["channelSlug"] = query.channelSlug();
	map["offset"] = QString::number(query.offset());
	map["limit"] = QString::number(query.limit());
	map["mediaType"] = query.mediaType();
	Query(modelData, QString("Playlists.GetPlaylistForChannel"), map, m_sessionInfo);
}


#if 0

/**
 * This method returns information about the current token.
 * Response:
 * <response status="ok">
 *   <tokeninfo id="TOKENID" applicationId=”APP_ID" expires="EXPIRATION" />
 * </response>
 */
QDomDocument Mediafly::Authentication_GetTokenInfo (const Mediafly::SessionInfo& session) {
	QMap<QString, QString> map;
	QDomDocument doc = Query("Authentication.GetTokenInfo", map, session);
	checkResponse(doc);
	return doc;
}

/**
 * This method returns a unique, user-friendly code that the user can enter into
 * the Mediafly system to associate this device or application with his Mediafly
 * account.  This allows the user to customize his Mediafly experience without
 * requiring access to the internal identifiers within the device or application. 
 * This code is good for 48 hours.
 * Response:
 * <response status="ok">
 *   <code value="CODE" />
 * </response>
 */
QDomDocument Mediafly::Authentication_GetUserAssociationCode (const Mediafly::SessionInfo& session) {
	QMap<QString, QString> map;
	QDomDocument doc = Query("Authentication.GetUserAssociationCode", map, session);
	checkResponse(doc);
	return doc;
}
#endif
/**
 * This method returns a list of account names bound to the calling device.
 * Response:
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="ok">
 *   <mfusers>
 *     <mfuser accountName="ACCOUNTNAME" default="false" />
 * 
 *   </mfusers>
 * </response>
 * Response (No bound account names):
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="ok">
 *   <mfusers />
 * </response>
 */
void Mediafly::Authentication_GetBoundMFUsers (mf::auth::GetBoundMFUsersData* data)
{
	QMap<QString, QString> map;
	Query(data, "Authentication.GetBoundMFUsers", map, m_sessionInfo);
}

/**
 * Call this method to add an account to a list of users that can have a
 * personalized experience with a device. The call must be made over a secure
 * channel to succeed.
 * Response (calling over HTTP):
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="fail">
 *   <err code="14" message="Request must be made over a secure connection." />
 * </response>
 * Response (invalid account name and/or password):
 *  <?xml version="1.0" encoding="utf-8"?>
 * <response status="fail">
 *   <err code="6" message="No such username/password" />
 * </response>
 * Response (binding an additional account when does NOT allow multiple users):
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="fail">
 *   <err code="302" message="Application context does not allow multiple user
 * bindings." />
 * </response>
 * Response (successful binding):
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="ok" />
 * @param  accountName account name of a user that you wish to bind to the device
 * @param  password
 * @param  default (optional): if set to “yes” sets the new account as the default
 * account to be used. If there are any other accounts bound to the device their
 * default status is turned off.
 */
void Mediafly::Authentication_BindMFUser (mf::auth::BindMFUserData* data, QString accountName, QString password, bool deflt)
{
	QMap<QString, QString> map;
	map["accountName"] = accountName;
	map["password"] = password;
	map["default"] = deflt ? "true" : "false";
	Query(data, QString("Authentication.BindMFUser"), map, m_sessionInfo, true);
}

/**
 * This call will mark a specified user as a default user to be used across the API
 * calls that require user context. The user that is set as default has to already
 * be bound to the device.
 * Response (trying to set unbound user as default):
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="fail">
 *   <err code="301" message="No such user binding found." />
 * </response>
 * Response:
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="ok" />
 * @param  accountName account name already bound to the device that should be set
 * as default.
 */
void Mediafly::Authentication_SetMFUserAsDefault (mf::auth::SetMFUserAsDefaultData *data,
                                                  QString accountName)
{
	QMap<QString, QString> map;
	map["accountName"] = accountName;
	Query(data, QString("Authentication.SetMFUserAsDefault"), map, m_sessionInfo);
}

/**
 * Call this method to remove a user from a device personalization. The user must
 * be already bound to the device for the call to succeed.
 * Response (trying to unbind not bound account):
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="fail">
 *   <err code="301" message="No such user binding found." />
 * </response>
 * Response:
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="ok" />
 */
void Mediafly::Authentication_UnbindMFUser (mf::CheckResponseOk *data, QString accountName)
{
	QMap<QString, QString> map;
	map["accountName"] = accountName;
	Query(data, QString("Authentication.UnbindMFUser"), map, m_sessionInfo);
}
#if 0
/**
 * This method returns the next channel from the user’s playlist, relative to the
 * specified channel.
 * Response:
 * <response status="ok">
 *   <channelResponse status="ok">
 * 
 *     <channel name="" slug="" />
 *   </channelResponse>
 * </response>
 * Response (Looped to Beginning of Channel List):
 * <response status="ok">
 *   <channelResponse status="looped">
 *     <channel name="" slug="" />
 *   </channelResponse>
 * </response>
 * Response (Empty Channel List):
 * <response status="ok">
 *   <channelResponse status="empty" />
 * </response>
 * @param  channelSlug (optional): the slug of the current channel.  If this is not
 * specified, or the channel could not be found, this method will return the first
 * channel in the playlist.
 * @param  capitalizeChannelNames (optional):  whether channel names will be
 * capitalized (defaults to “true”, excepts values “true” or “false”)
 */
QDomDocument Mediafly::Playlists_GetNextChannelForPlaylist (const Mediafly::SessionInfo& session, QString channelSlug, bool capitalizeChannelNames) {
	QMap<QString, QString> map;
	map["channelSlug"] = channelSlug;
	map["capitalizeChannelNames"] = capitalizeChannelNames ? "true" : "false";
	QDomDocument doc = Query("Playlists.GetNextChannelForPlaylist", map, session);
	checkResponse(doc);
	return doc;
}


/**
 * This method returns the previous channel from the user’s playlist, relative to
 * the specified channel.
 * Response:
 * <response status="ok">
 *   <channelResponse status="ok">
 *     <channel name="" slug="" />
 *   </channelResponse>
 * </response>
 * Response (Looped to End of Channel List):
 * <response status="ok">
 *   <channelResponse status="looped">
 *     <channel name="" slug="" />
 *   </channelResponse>
 * </response>
 * Response (Empty Channel List):
 * <response status="ok">
 *   <channelResponse status="empty" />
 * </response>
 * @param  channelSlug (optional):  the slug of the current channel.  If this is
 * not specified, or the channel could not be found, this method will return the
 * last channel in the playlist.
 * @param  capitalizeChannelNames (optional):  whether channel names will be
 * capitalized (defaults to “true”, excepts values “true” or “false”)
 */
QDomDocument Mediafly::Playlists_GetPrevChannelForPlaylist (const Mediafly::SessionInfo& session, QString channelSlug, bool capitalizeChannelNames) {
	QMap<QString, QString> map;
	map["channelSlug"] = channelSlug;
	map["capitalizeChannelNames"] = capitalizeChannelNames ? "true" : "false";
	QDomDocument doc = Query("Playlists.GetPrevChannelForPlaylist", map, session);
	checkResponse(doc);
	return doc;
}


/**
 * This method returns the next episode in the specified channel relative to the
 * specified episode from the user’s playlist.
 * Response:
 * <response status="ok">
 *   <episodeResponse status="ok">
 *     <episode slug="" title=”” description="" format=““ url=““ urlOriginal=““
 * published=""
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </episodeResponse>
 * </response>
 * Response (Looped to End of Channel Playlist):
 * <response status="ok">
 *   <episodeResponse status="looped">
 *     <episode slug=““ title=”” description="" format=““ url=““ urlOriginal=““
 * published=""
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </episodeResponse>
 * </response>
 * Response (Looped to End of Channel Playlist):
 * <response status="ok">
 *   <episodeResponse status="empty" />
 * </response>
 * @param  channelSlug (required):  the slug of the current channel
 * @param  episodeSlug (optional):  the slug of the current episode.  If this is
 * not specified, or the specified episode cannot be found, this method will return
 * the first episode in the user’s playlist.
 * @param  mediaType (optional):  include or exclude content based on its type
 * (“audio”, “video”, default: “audio,video”)
 */
QDomDocument Mediafly::Playlists_GetNextEpisodeForChannelPlaylist (const Mediafly::SessionInfo& session, QString channelSlug, QString episodeSlug, QString mediaType) {
	QMap<QString, QString> map;
	map["channelSlug"] = channelSlug;
	map["episodeSlug"] = episodeSlug;
	map["mediaType"] = mediaType;
	QDomDocument doc = Query("Playlists.GetNextEpisodeForChannelPlaylist", map, session);
	checkResponse(doc);
	return doc;
}


/**
 * This method returns the previous episode in the specified channel relative to
 * the specified episode from the user’s playlist.
 * Response:
 * <response status="ok">
 *   <episodeResponse status="ok">
 *     <episode slug=““ title=”” description="" format=““ url=““ urlOriginal=““
 * published=""
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </episodeResponse>
 * </response>
 * Response (Looped to Start of Channel Playlist):
 * <response status="ok">
 *   <episodeResponse status="looped">
 *     <episode slug=”” title=”” description="" format=““ url=““ urlOriginal=““
 * published=""
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </episodeResponse>
 * </response>
 * Response (Looped to End of Channel Playlist):
 * <response status="ok">
 *   <episodeResponse status="empty" />
 * </response>
 * @param  channelSlug  (required):  the slug of the current channel
 * @param  episodeSlug (optional):  the slug of the current episode.  If this is
 * not specified, or the specified episode cannot be found, this method will return
 * the last episode in the user’s playlist.
 * @param  mediaType  (optional):  include or exclude content based on its type
 * (“audio”, “video”, default: “audio,video”)
 */
QDomDocument Mediafly::Playlists_GetPrevEpisodeForChannelPlaylist (const Mediafly::SessionInfo& session, QString channelSlug, QString episodeSlug, QString mediaType ) {
	QMap<QString, QString> map;
	map["channelSlug"] = channelSlug;
	map["episodeSlug"] = episodeSlug;
	map["mediaType"] = mediaType;
	QDomDocument doc = Query("Playlists.GetPrevEpisodeForChannelPlaylist", map, session);
	checkResponse(doc);
	return doc;
}
#endif

/**
 * This method removes an episode from user’s playlists.
 * Response:
 * <response status="ok" />
 * @param  episodeSlug (required):  the slug of the episode that you want to remove
 * from the playlists.
 */
void Mediafly::Playlists_RemoveEpisodeFromPlaylist (mf::CheckResponseOk *data, QString episodeSlug)
{
	QMap<QString, QString> map;
	map["episodeSlug"] = episodeSlug;
	Query(data, "Playlists.RemoveEpisodeFromPlaylist", map, m_sessionInfo);
}

#if 0
/**
 * This method returns a list of episodes for the specified show.
 * Response:
 * <response status="ok">
 * 
 *   <playlist showSlug="business" totalEpisodes="3">
 *     <episode slug=““ title="" description=”” format="" url="" urlOriginal=""
 * published=””
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *     <episode slug=““ title="" description=”” format="" url="" urlOriginal=""
 * published=””
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *     <episode slug=““ title="" description=”” format="" url="" urlOriginal=""
 * published=””
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </playlist>
 * </response>
 * Response (No Episodes):
 * <response status="ok">
 *   <playlist showSlug="business" />
 * </response>
 * @param  showSlug (required):  the slug for the requested show
 * @param  offset  (optional):  an integer value representing the offset for the
 * start of the playlist results (default: 0)
 * @param  limit (optional): an integer value representing the number of episodes
 * to return (default:500, max:500)
 * @param  mediaType  (optional):  include or exclude content based on its type
 * (“audio”, “video”, default: “audio,video”)
 */
QDomDocument Mediafly::Shows_GetEpisodesForShow (const Mediafly::SessionInfo& session, QString showSlug, int offset, int limit, QString mediaType ) {
	QMap<QString, QString> map;
	map["showSlug"] = showSlug;
	map["offset"] = QString::number(offset);
	map["limit"] = QString::number(limit);
	map["mediaType"] = mediaType;
	QDomDocument doc = Query("Shows.GetEpisodesForShow", map, session);
	checkResponse(doc);
	return doc;
}


/**
 * This method returns the next episode in the specified show relative to the
 * specified episode from the user’s playlist.
 * Response:
 * <response status="ok">
 *   <episodeResponse status="ok">
 *     <episode slug="" title=”” description="" format=““ url=““ urlOriginal=““
 * published=""
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </episodeResponse>
 * </response>
 * Response (Looped to End of Show Playlist):
 * <response status="ok">
 *   <episodeResponse status="looped">
 *     <episode slug=““ title=”” description="" format=““ url=““ urlOriginal=““
 * published=""
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </episodeResponse>
 * </response>
 * Response (Looped to End of Show Playlist):
 * <response status="ok">
 *   <episodeResponse status="empty" />
 * </response>
 * @param  showSlug (required):  the slug of the current show
 * @param  episodeSlug (optional):  the slug of the current episode.  If this is
 * not specified, or the specified episode cannot be found, this method will return
 * the first episode in the user’s playlist.
 * @param  mediaType  (optional):  include or exclude content based on its type
 * (“audio”, “video”, default: “audio,video”)
 */
QDomDocument Mediafly::Shows_GetNextEpisodeForShow (const Mediafly::SessionInfo& session, QString showSlug, QString episodeSlug, QString mediaType ) {
	QMap<QString, QString> map;
	map["showSlug"] = showSlug;
	map["episodeSlug"] = episodeSlug;
	map["mediaType"] = mediaType;
	QDomDocument doc = Query("Shows.GetNextEpisodeForShow", map, session);
	checkResponse(doc);
	return doc;
}


/**
 * This method returns the previous episode in the specified channel relative to
 * the specified episode from the user’s playlist.
 * Response:
 * <response status="ok">
 *   <episodeResponse status="ok">
 *     <episode slug=““ title=”” description="" format=““ url=““ urlOriginal=““
 * published=""
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </episodeResponse>
 * </response>
 * Response (Looped to Start of Show Playlist):
 * <response status="ok">
 *   <episodeResponse status="looped">
 *     <episode slug=”” title=”” description="" format=““ url=““ urlOriginal=““
 * published=""
 * showSlug=”” showTitle=”” imageUrl=”” channel=”” />
 *   </episodeResponse>
 * </response>
 * Response (Looped to End of Show Playlist):
 * <response status="ok">
 *   <episodeResponse status="empty" />
 * </response>
 * @param  showSlug
 * @param  episodeSlug
 * @param  mediaType
 */
QDomDocument Mediafly::Shows_GetPrevEpisodeForShow (const Mediafly::SessionInfo& session, QString showSlug, QString episodeSlug, QString mediaType ) {
	QMap<QString, QString> map;
	map["showSlug"] = showSlug;
	map["episodeSlug"] = episodeSlug;
	map["mediaType"] = mediaType;
	QDomDocument doc = Query("Shows.GetPrevEpisodeForShow", map, session);
	checkResponse(doc);
	return doc;
}
#endif

/**
 * This method allows you to post real time playback information up to the Mediafly
 * servers.  This allows us to adapt the users’ playlist and react to the content
 * as they experience it.
 * Response:
 * 
 * <response status="ok" />
 * @param  episodeSlug
 * @param  position
 * @param  episodeLength
 */
void Mediafly::Experience_PostExperienceForEpisode (mf::CheckResponseOk *data, QString episodeSlug, int position, int episodeLength)
{
	QMap<QString, QString> map;
	map["episodeSlug"] = episodeSlug;
	map["position"] = QString::number(position);
	map["episodeLength"] = QString::number(episodeLength);
	Query(data, "Experience.PostExperienceForEpisode", map, m_sessionInfo);
}

#if 0
/**
 * This method returns a list of shows and/or episodes based on the specified
 * search term.  Please note: shows include additional attributes in the result set
 * describing the most recent episode for that show.
 * Response:
 * <response status="ok">
 *   <searchResults>
 *     <episode slug="" title="" description="" format="" url="" urlOriginal=""
 * published=""
 * channel="" showSlug="" showTitle="" imageUrl=”” />
 *     <show slug="" title=" description="" url="" urlOriginal="" channel=""
 * episodeTitle=""
 * episodeDescription="" episodeSlug="" episodeUrl="" episodeOriginalUrl=""
 * episodePublished="" imageUrl=”” />
 *   </searchResults>
 * </response>
 * Response With Counts Included:
 * <response status="ok">
 *   <searchResults totalResults=”5” totalShows=”1” totalEpisodes=”4”>
 *     <episode slug="" title="" description="" format="" url="" urlOriginal=""
 * published=""
 * channel="" showSlug="" showTitle="" imageUrl=”” />
 *     <show slug="" title=" description="" url="" urlOriginal="" channel=""
 * episodeTitle=""
 * episodeDescription="" episodeSlug="" episodeUrl="" episodeOriginalUrl=""
 * episodePublished="" imageUrl=”” />
 *   </searchResults>
 * </response>
 * @param  term  (required):  a phrase used to search the database (e.g. “comedy”
 * or “political news”)
 * @param  searchType (optional):  a comma separated list of entity types to search
 * (possible values: “show” and “episode”, defaults to “show”)
 * @param  explicitFilter (optional):  include or exclude explicit content
 * (possible values: “notexplicit”, “explicit” and “all”, defaults to
 * “notexplicit”)
 * @param  mediaType
 * @param  offset
 * @param  limit
 * @param  includeCounts (optional): if this is set to “yes”, the result set will
 * include three additional attributes (totalResults, totalShows, and
 * totalEpisodes) that include the counts of all available entries that match the
 * search query (regardless of offset and limit settings).  This option noticeably
 * increases the time it takes to calculate the search results and is therefore
 * discouraged.
 */
QDomDocument Mediafly::Search_Query (const Mediafly::SessionInfo& session, QString term, QString searchType, QString explicitFilter, QString mediaType, int offset, int limit, bool includeCounts ) {
	QMap<QString, QString> map;
	map["term"] = term;
	map["searchType"] = searchType;
	map["explicitFilter"] = explicitFilter;
	map["mediaType"] = mediaType;
	map["offset"] = QString::number(offset);
	map["limit"] = QString::number(limit);
	map["includeCounts"] = includeCounts ? "yes" : "no";
	QDomDocument doc = Query("Search.Query", map, session);
	checkResponse(doc);
	return doc;
}


/**
 * This method subscribes a user to the specified show or episode.  This method
 * only works if the device is associated with a Mediafly user account.
 * Response (success):
 * <response status="ok" />
 * Response (invalid user):
 * <response status="fail">
 *   <err code="12" message="this device is not associated with a user" />
 * </response>
 * Response (invalid episode):
 * <response status="fail">
 *   <err code="202" message="invalid episode" />
 * </response>
 * Response (invalid show):
 * <response status="fail">
 *   <err code="201" message="invalid show" />
 * </response>
 * @param  slug
 * @param  subscriptionType  (optional): subscribe to a “show” or an “episode”
 * (default: “show”)
 * @param  channel (optional):  the channel this show or episode will be subscribed
 * in.  The default is to use server side logic to determine the channel
 * automatically.
 */
QDomDocument Mediafly::Subscriptions_Subscribe (const Mediafly::SessionInfo& session, QString slug, QString subscriptionType, QString channel ) {
	QMap<QString, QString> map;
	map["slug"] = slug;
	map["subscriptionType"] = subscriptionType;
	map["channel"] = channel;
	QDomDocument doc = Query("Subscriptions.Subscribe", map, session);
	checkResponse(doc);
	return doc;
}


/**
 * This method unsubscribes a user from the specified show or episode.  This method
 * only works if the device is associated with a Mediafly user account.
 * Response (success):
 * <response status="ok" />
 * Response (invalid user):
 * 
 * <response status="fail">
 *   <err code="12" message="this device is not associated with a user" />
 * </response>
 * Response (invalid episode):
 * <response status="fail">
 *   <err code="202" message="invalid episode" />
 * </response>
 * Response (invalid show):
 * <response status="fail">
 *   <err code="201" message="invalid show" />
 * </response>
 * @param  slug
 * @param  subscriptionType (optional):  unsubscribe from a “show” or an “episode”
 * (default: “show”)
 */
QDomDocument Mediafly::Subscriptions_Unsubscribe (const Mediafly::SessionInfo& session, QString slug, QString subscriptionType ) {
	QMap<QString, QString> map;
	map["slug"] = slug;
	map["subscriptionType"] = subscriptionType;
	QDomDocument doc = Query("Subscriptions.Unsubscribe", map, session);
	checkResponse(doc);
	return doc;
}


/**
 * This method returns information for an episode in the Mediafly system.
 * Response (success):
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="ok">
 *   <episodeResponse status="ok">
 *     <episode slug="SLUG" title="TITLE" ... />
 *   </episodeResponse>
 * </response>
 * Response (episode not found):
 * <?xml version="1.0" encoding="utf-8"?>
 * <response status="fail">
 *   <err code="400" message="Episode not found" />
 * </response>
 * @param  episodeSlug
 */
QDomDocument Mediafly::Episodes_GetEpisodeInfo (const Mediafly::SessionInfo& session, QString episodeSlug ) {
	QMap<QString, QString> map;
	map["episodeSlug"] = episodeSlug;
	QDomDocument doc = Query("Episodes.GetEpisodeInfo", map, session);
	checkResponse(doc);
	return doc;
}

#endif

