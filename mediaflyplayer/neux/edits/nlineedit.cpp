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
 * NlineEdit editbox routines.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-21 SZ
 *
 */
#include "nlineedit.h"

NLineEdit :: NLineEdit ( QWidget * parent )
	: QLineEdit ( parent )
{
	isNumberOnly = false;
	tip = tr("Waiting for your input...");
}

void NLineEdit::showEvent(QShowEvent *e)
{
	QLineEdit::showEvent(e);
	if (isNumberOnly)
		setAttribute(Qt::WA_InputMethodEnabled,false);
}

void NLineEdit :: keyPressEvent ( QKeyEvent *e )
{
	/* Fix Me! This should be removed when osd ir number key
		handle bug is fixed.*/
	if (isNumberOnly)
	{
		switch ( e->key ( ))
		{
		case Qt::Key_Left:
			cursorBackward(false);
			break;
		case Qt::Key_Right:
			cursorForward(false);
			break;
		case Qt::Key_Clear:
			backspace ( );
			break;
		case Qt::Key_0:
			insert ("0" );
			break;
		case Qt::Key_1:
			insert ("1" );
			break;
		case Qt::Key_2:
			insert ("2" );
			break;
		case Qt::Key_3:
			insert ("3" );
			break;
		case Qt::Key_4:
			insert ("4" );
			break;
		case Qt::Key_5:
			insert ("5" );
			break;
		case Qt::Key_6:
			insert ("6" );
			break;
		case Qt::Key_7:
			insert ("7" );
			break;
		case Qt::Key_8:
			insert ("8" );
			break;
		case Qt::Key_9:
			insert ("9" );
			break;
		}
	}
	e->ignore();
	return QLineEdit :: keyPressEvent ( e );
}

QVariant NLineEdit::inputMethodQuery ( Qt::InputMethodQuery query ) const
{
	if (query == Qt::ImCurrentSelection)
		return QStringList() << selectedText() << toolTip();
	else
		return QLineEdit::inputMethodQuery(query);
}

void NLineEdit::inputMethodEvent(QInputMethodEvent *e)
{
	QLineEdit::inputMethodEvent(e);

	QKeyEvent keyEvent = QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
	QApplication::sendEvent(this, &keyEvent);
}

