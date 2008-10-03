#ifndef _NLINEEDIT_H__
#define _NLINEEDIT_H__
/*
 *  Copyright(C) 2007 Neuros Technology International LLC. 
 *               <www.neurostechnology.com>
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
 ****************************************************************************
 *
 *  NlineEdit editbox header.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-21 SZ
 *
 */


#include <QLineEdit>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>
class NLineEdit : public QLineEdit
{
    Q_OBJECT

public:
	NLineEdit ( QWidget * parent = 0 );

	void setNumberOnly(bool numberOnly) {isNumberOnly = numberOnly;}
	bool numberOnly() const {return isNumberOnly;}

	const QString toolTip() const	{return tip;}
	void setToolTip (const QString &tooltip){ tip = tooltip;}

protected:
	void keyPressEvent ( QKeyEvent * );
	void showEvent(QShowEvent *);

	QVariant inputMethodQuery ( Qt::InputMethodQuery query ) const;

	void inputMethodEvent(QInputMethodEvent *);
private:
	bool isNumberOnly;
	
	QString tip;

};

#endif
