#include <QString>

namespace mf {

class SessionInfo
{
	QString m_token;
	QString m_tokenId;
public:
	SessionInfo() { }

	SessionInfo(const QString& token, const QString& tokenId) :
		m_token (token), m_tokenId (tokenId) { }

	const QString& token() const { return m_token; }
	const QString& tokenId() const { return m_tokenId; }

	void setToken(const QString& token) { m_token = token; }
	void setTokenId(const QString& tokenId) { m_tokenId = tokenId; }
};

}

