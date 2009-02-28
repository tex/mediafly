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

#ifndef mfChannelEntry_H
#define mfChannelEntry_H

#include <QString>
#include <QMetaType>

namespace mf {

class ChannelEntry
{
public:
	ChannelEntry()
	{
		qRegisterMetaType<ChannelEntry>("mf::ChannelEntry");
	}
	ChannelEntry(const ChannelEntry& obj) :
		m_name (obj.m_name),
		m_slug (obj.m_slug)
	{
	}

	ChannelEntry(QString name, QString slug) :
		m_name (name),
		m_slug (slug)
	{
		qRegisterMetaType<ChannelEntry>("mf::ChannelEntry");
	}

	const QString& name() const { return m_name; }
	const QString& slug() const { return m_slug; }

private:
	QString m_name;
	QString m_slug;
};

}

Q_DECLARE_METATYPE(mf::ChannelEntry);

#endif

