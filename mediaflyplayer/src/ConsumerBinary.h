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

#ifndef mfConsumerBinary_H
#define mfConsumerBinary_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QPixmap>

namespace mf {

class ConsumerBinary : public QObject
{
	Q_OBJECT
public:
	void read(const QString& path, const QByteArray& array)
	{
		qDebug() << __PRETTY_FUNCTION__ << path;

		emit binaryRead(path, array);
	}

signals:
	void binaryRead(const QString& path, const QByteArray& array);
};

}

#endif

