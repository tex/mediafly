#include "Mediafly.h"
#include "MediaflyEpisodeModel.h"

MediaflyEpisodeModel::MediaflyEpisodeModel(QObject *parent) :
	QAbstractListModel(parent)
{
	qRegisterMetaType<MediaflyEpisodeModel>("MediaflyEpisodeModel");

	connect(&m_episodeModelThread, SIGNAL(refreshed(const MediaflyEpisodeModel&)),
	        this, SLOT(handleRefreshed(const MediaflyEpisodeModel&)));
	connect(&m_episodeModelThread, SIGNAL(error(const QString&)),
	        this, SLOT(handleError(const QString&)));
}

MediaflyEpisodeModel::MediaflyEpisodeModel(const MediaflyEpisodeModel& obj) :
	QAbstractListModel(dynamic_cast<const QObject&>(obj).parent())
{
	m_data = obj.m_data;
}

void MediaflyEpisodeModel::refresh(QString channelSlug, int offset, int limit, QString mediaType)
{
	m_episodeModelThread.refresh(channelSlug, offset, limit, mediaType);
}

void MediaflyEpisodeModel::handleError(const QString& errorMsg)
{
	emit error(errorMsg);
}

void MediaflyEpisodeModel::handleRefreshed(const MediaflyEpisodeModel& obj)
{
	// TODO: unite may insert already existing keys multiple times,
	// is it a problem for us?

	m_data.unite(obj.m_data);

	emit refreshed();
}

void MediaflyEpisodeModel::readData(QString channelSlug, int offset, int limit, QString mediaType)
{
	Mediafly mf("dfcfefff34d0458fa3df0e0c7a6feb6c", "N38r0s0sd");
	Mediafly::SessionInfo session = mf.Authentication_GetToken("123");
	QDomDocument doc = mf.Playlists_GetPlaylistForChannel(session, channelSlug, offset, limit, mediaType);

	QDomNode it = doc.firstChildElement("response").firstChildElement("playlist").firstChild();
	while (!it.isNull()) {
		QDomElement el = it.toElement();
		if (!el.isNull()) {
			QMap<int, QString> data;
			data[titleRole]       = el.attribute("title");
			data[slugRole]        = el.attribute("slug");
			data[descriptionRole] = el.attribute("description");
			data[formatRole]      = el.attribute("format");
			data[urlRole]         = el.attribute("url");
			data[urlOriginalRole] = el.attribute("urlOriginal");
			data[publishedRole]   = el.attribute("published");
			data[showSlugRole]    = el.attribute("showSlug");
			data[showTitleRole]   = el.attribute("showTitle");
			data[imageUrlRole]    = el.attribute("imageUrl");
			data[channelRole]     = el.attribute("channel");
			m_data[offset] = data;
		}
		it = it.nextSibling();
		++offset;
	}
}

int MediaflyEpisodeModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_data.size();
}

QVariant MediaflyEpisodeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_data.size())
		return QVariant();

	switch (role) {
	case titleRole:       return (m_data[index.row()])[titleRole];
	case slugRole:        return (m_data[index.row()])[slugRole];
	case descriptionRole: return (m_data[index.row()])[descriptionRole];
	case formatRole:      return (m_data[index.row()])[formatRole];
	case urlRole:         return (m_data[index.row()])[urlRole];
	case urlOriginalRole: return (m_data[index.row()])[urlOriginalRole];
	case publishedRole:   return (m_data[index.row()])[publishedRole];
	case showSlugRole:    return (m_data[index.row()])[showSlugRole];
	case showTitleRole:   return (m_data[index.row()])[showTitleRole];
	case imageUrlRole:    return (m_data[index.row()])[imageUrlRole];
	case channelRole:     return (m_data[index.row()])[channelRole];
	default:              return QVariant();
	}
}

QString MediaflyEpisodeModel::toString() const
{
	QString text = "(";

	for (int i = 0; i < m_data.size(); ++i)
	{
		text += ((m_data[i])[titleRole]);
		if (i + 1 < m_data.size())
			text += ", ";
	}
	text += ")";

	return text;
}

