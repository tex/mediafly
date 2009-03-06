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

#ifndef mfPersonalizeSimple_H
#define mfPersonalizeSimple_H

#include "ui_MediaflyPersonalizeSimple.h"
#include "GetUserAssociationCodeData.h"

#include <QString>

namespace mf {

class PersonalizeSimple : public QWidget, private Ui::MediaflyPersonalizeSimple
{
	Q_OBJECT
public:
	PersonalizeSimple(QWidget *parent = 0);
	void show();

signals:
	void back();

private:
	mf::auth::GetUserAssociationCodeData m_data;
};

}

#endif

