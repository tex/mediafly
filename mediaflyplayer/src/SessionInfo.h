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

