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

#ifndef mfQuery_H
#define mfQuery_H

namespace mf {

class Query
{
public:
	Query(int offset = 0, int limit = 0) :
		m_offset(offset),
		m_limit(limit)
	{ }

	Query(const Query& obj)
	{
		if (this == &obj)
			return;
		m_offset = obj.m_offset;
		m_limit = obj.m_limit;
	}

	void advanceOffset() { m_offset += m_limit; }

	int offset() const { return m_offset; }
	int limit() const { return m_limit; }

protected:
	int     m_offset;
	int     m_limit;
};

}

#endif

