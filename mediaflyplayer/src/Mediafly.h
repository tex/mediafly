/************************************************************************
**************************************************************************/

#ifndef MEDIAFLY_H
#define MEDIAFLY_H

#include <QMap>
#include <QString>

#include "Exception.h"
#include "REST.h"

/**
  * class Mediafly
  */

class Mediafly : public REST
{
public:
	class SessionInfo
	{
		QString m_token;
		QString m_tokenId;
	public:
		SessionInfo(const QString& token, const QString& tokenId) :
			m_token (token), m_tokenId (tokenId) { }
		const QString& token() const { return m_token; }
		const QString& tokenId() const { return m_tokenId; }
	};

public:
	class InvalidDocumentException : public Exception
	{
	public:
		InvalidDocumentException(QString /*document*/) : Exception("Invalid document") { }
	};
	class FailedResponseException : public Exception
	{
	public:
		FailedResponseException(QString /*errCode*/, QString errMessage) : Exception("Failed response: " + errMessage) { }
	};

	Mediafly(QString appId, QString sharedSecret);

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
	 * @return session token
	 */
	SessionInfo Authentication_GetToken (QString thirdPartyUserID );

	/**
	 * This method returns information about the current token.
	 * Response:
	 * <response status="ok">
	 *   <tokeninfo id="TOKENID" applicationId=”APP_ID" expires="EXPIRATION" />
	 * </response>
	 */
	QDomDocument Authentication_GetTokenInfo (const Mediafly::SessionInfo& session);

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
	QDomDocument Authentication_GetUserAssociationCode (const Mediafly::SessionInfo& session);

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
	QDomDocument Authentication_GetBoundMFUsers (const Mediafly::SessionInfo& session);

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
	QDomDocument Authentication_BindMFUser (const Mediafly::SessionInfo& session, QString accountName, QString password, bool deflt = false);

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
	QDomDocument Authentication_SetMFUserAsDefault (const Mediafly::SessionInfo& session, QString accountName);

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
	QDomDocument Channels_UnbindMFUser (const Mediafly::SessionInfo& session);

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
	QDomDocument Channels_GetChannels (const Mediafly::SessionInfo& session, bool capitalizeChannelNames);

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
	QDomDocument Playlists_GetPlaylistForChannel (const Mediafly::SessionInfo& session, QString channelSlug, int offset, int limit, QString mediaType = "audio,video");

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
	QDomDocument Playlists_GetNextChannelForPlaylist (const Mediafly::SessionInfo& session, QString channelSlug, bool capitalizeChannelNames);

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
	QDomDocument Playlists_GetPrevChannelForPlaylist (const Mediafly::SessionInfo& session, QString channelSlug, bool capitalizeChannelNames );

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
	QDomDocument Playlists_GetNextEpisodeForChannelPlaylist (const Mediafly::SessionInfo& session, QString channelSlug, QString episodeSlug, QString mediaType);


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
	QDomDocument Playlists_GetPrevEpisodeForChannelPlaylist (const Mediafly::SessionInfo& session, QString channelSlug, QString episodeSlug, QString mediaType );


	/**
	 * This method removes an episode from user’s playlists.
	 * Response:
	 * <response status="ok" />
	 * @param  episodeSlug (required):  the slug of the episode that you want to remove
	 * from the playlists.
	 */
	QDomDocument Playlists_RemoveEpisodeFromPlaylist (const Mediafly::SessionInfo& session, QString episodeSlug );


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
	QDomDocument Shows_GetEpisodesForShow (const Mediafly::SessionInfo& session, QString showSlug, int offset, int limit, QString mediaType );


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
	QDomDocument Shows_GetNextEpisodeForShow (const Mediafly::SessionInfo& session, QString showSlug, QString episodeSlug, QString mediaType );


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
	QDomDocument Shows_GetPrevEpisodeForShow (const Mediafly::SessionInfo& session, QString showSlug, QString episodeSlug, QString mediaType );


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
	QDomDocument Experience_PostExperienceForEpisode (const Mediafly::SessionInfo& session, QString episodeSlug, int position, int episodeLength );


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
	QDomDocument Search_Query (const Mediafly::SessionInfo& session, QString term, QString searchType, QString explicitFilter, QString mediaType, int offset, int limit, bool includeCounts = false );


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
	QDomDocument Subscriptions_Subscribe (const Mediafly::SessionInfo& session, QString slug, QString subscriptionType, QString channel );


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
	QDomDocument Subscriptions_Unsubscribe (const Mediafly::SessionInfo& session, QString slug, QString subscriptionType );


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
	QDomDocument Episodes_GetEpisodeInfo (const Mediafly::SessionInfo& session, QString episodeSlug );

private:
	QString m_appId;
	QString m_sharedSecret;

	void checkResponse(QDomDocument& doc);
	QString computeHash(QMap<QString, QString>& map, QString token_id);
	QStringList makeParams(QMap<QString, QString>& map);
	QDomDocument Query(QString function, QMap<QString, QString>& map);
	QDomDocument Query(QString function, QMap<QString, QString>& map, const Mediafly::SessionInfo& session);
};

#endif // MEDIAFLY_H
