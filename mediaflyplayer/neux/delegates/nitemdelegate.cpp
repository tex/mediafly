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
 * neux NListView delegate routine.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-12-14 SZ
 *
 */
#include "nitemdelegate.h"
#include "nlistview.h"
#include "nlineedit.h"

NItemDelegate :: NItemDelegate ( QWidget *parent )
	: QItemDelegate( parent ) , ShowIcon(true),ShowRightIcon(true) , isSetData ( true ),
    curEditor(NULL) , ShowLeftArrowIcon(true), isClickFlashActive(false)
{

 }

QWidget *NItemDelegate ::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    NLineEdit *editor = new NLineEdit();
	editor->setToolTip(tr("Input New Name"));

	QPoint globalPos = parent->mapToGlobal(QPoint(option.rect.x( ) ,option.rect.y( )) ); ;

	editor->setGeometry(globalPos.x( ) , globalPos.y( )  , 
						option.rect.width( ) , option.rect.height( ));

	QFont  f; 
	f.setPointSize(option.font.pointSize( ));
	editor->setFont ( f );

    curEditor = editor;
	return editor;
}

void NItemDelegate :: setEditorText(const QString &text)
{
    NLineEdit *editor = qobject_cast<NLineEdit *>(curEditor);
    if (NULL != editor)
    {
        editor->setText(text);
    }
}

void NItemDelegate ::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
     NLineEdit *editbox = static_cast<NLineEdit*>(editor);

	 editbox->setText( index.data( ).toString( ) );
}

void NItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
     NLineEdit *editbox = static_cast<NLineEdit*>(editor);

     emit editorAboutToClosed( index , editbox->text( ) );

     if ( isSetData)
     {
         int status = model->setData(index, editbox->text( ), Qt::EditRole);
         emit editorClosed ( index , editbox->text () ,status);
     }
     isSetData = true;

}

void NItemDelegate :: onEnableEditorSetData ( bool enable )
{
    isSetData = enable;
}

void NItemDelegate :: onEnableInputMethod ( bool enable )
{
    if ( curEditor )
    {
        curEditor->setAttribute( Qt::WA_InputMethodEnabled , enable );
    }
}


void NItemDelegate :: updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}

void  NItemDelegate :: paint(QPainter *painter, const QStyleOptionViewItem &option,
          const QModelIndex &index) const
{
    if (option.state & QStyle::State_HasFocus || 
        (!(option.state & QStyle::State_Active) && index == focusIndex))
     {
          focusIndex = index;
          drawFocus(painter, option);
          drawLeftArrowIcon (painter, option, index);
          drawRightIcon (painter, option, index);
          drawDisplay(painter, option, index, true);
     }
    else 
    {
        drawDisplay(painter, option, index, false);
    }

    drawSelection (painter, option, index);
    drawIcon (painter, option, index);
}

QSize NItemDelegate::sizeHint ( const QStyleOptionViewItem & option, 
										const QModelIndex & index ) const  
{
    if ( ItemHintSize.height() < 0 && ItemHintSize.width() < 0)
    {
        if ( parent( ) )
        {
            QWidget *p = static_cast<QWidget*>(parent( ) );
            QFontMetrics fm = QFontMetrics(p->font());
            ItemHintSize = QSize(p->width( ) ,(fm.height()*5/3));
        }
        else
            ItemHintSize = QItemDelegate :: sizeHint ( option , index );
    }
    else if (ItemHintSize.width() < 0)
	{
        if ( parent( ) )
        {
            QWidget *p = static_cast<QWidget*>(parent( ) );
            ItemHintSize = QSize(p->width( ), ItemHintSize.height());
        }
        else
            ItemHintSize = QSize(QItemDelegate :: sizeHint ( option , index ).width(), ItemHintSize.height());
	}
	else if (ItemHintSize.height() < 0)
	{
        if ( parent( ) )
        {
            QWidget *p = static_cast<QWidget*>(parent( ) );
            QFontMetrics fm = QFontMetrics(p->font());
            ItemHintSize = QSize(p->width( ) ,(fm.height()*5/3));
        }
        else
            ItemHintSize = QSize(ItemHintSize.width(), QItemDelegate :: sizeHint ( option , index ).height());
	}
    return ItemHintSize;
}

QPainterPath NItemDelegate::prepareSelectionPath (const QRect& itemRect) const
{
	int radius = itemRect.height() / 5;
	int diam = 2 * radius;

	int x1, y1, x2, y2;
	itemRect.getCoords(&x1, &y1, &x2, &y2);

	QPainterPath path;
	path.moveTo(x2, y1 + radius);
	path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
	path.lineTo(x1 + radius, y1);
	path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
	path.lineTo(x1, y2 - radius);
	path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
	path.lineTo(x1 + radius, y2);
	path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
	path.closeSubpath();

	return path;
}

void NItemDelegate::drawFocus (QPainter *painter, 
                               const QStyleOptionViewItem &option) const
{
	QPainterPath path = prepareSelectionPath(option.rect);

	painter->save();
	painter->setClipPath(path);

	if (!isClickFlashActive)
	{
		QRect r(option.rect);
		int half = r.height() / 2;
		r.setBottom(r.bottom() - half);
		painter->fillRect(r, QColor(64, 64, 64));
		r.moveTop(r.top() + half);
		painter->fillRect(r, QColor(8, 8, 8));

		painter->strokePath(path, QPen(QColor(0xe4, 0x5b, 0x00).darker(125), 6));
	}
	else 
	{
		painter->fillRect(option.rect, QColor(0xe4, 0x5b, 0x00));
	}	
	painter->restore();
}

void NItemDelegate::drawLeftArrowIcon (QPainter *painter, 
                                       const QStyleOptionViewItem &option, 
                                       const QModelIndex &index) const
{
    /* draw left arrow icon */
    if ( ShowLeftArrowIcon )
    {
        QPixmap pixmap;

        if ( !QPixmapCache :: find ( LEFT_ARROW , pixmap) )
        {
            pixmap = QPixmap(LEFT_ARROW);
            QPixmapCache :: insert ( LEFT_ARROW , pixmap );
        }

        painter->drawPixmap(
                           option.rect.x() + LeftArrowIconPosition.x( )  ,
                           option.rect.y() + LeftArrowIconPosition.y( ) + ItemHintSize.height( )/2 - LeftArrowIconSize.height( )/2 , 
                           LeftArrowIconSize.width( ) , 
                           LeftArrowIconSize.height( ) ,
                           pixmap
                           );
    }

}

void NItemDelegate::drawRightIcon (QPainter *painter, 
                                   const QStyleOptionViewItem &option, 
                                   const QModelIndex &index) const
{
    /* draw right icon XIM / NAVRIGHT */
    if ( ShowRightIcon )
    {
        QString icon = index.data(NListView :: RightIconRole).toString();
        QPixmap pixmap;
        if ( !QPixmapCache :: find (icon , pixmap ) )
        {
            pixmap = QPixmap(icon);
            QPixmapCache :: insert ( icon , pixmap );
        }
        painter->drawPixmap(
                           option.rect.x()+RightIconPosition.x( ),
                           option.rect.y() + RightIconPosition.y( ) + ItemHintSize.height( )/2- RightIconSize.height( ) / 2 ,
                           RightIconSize.width(),
                           RightIconSize.height(),
                           pixmap
                           );
    }
}

void NItemDelegate::drawDisplay (QPainter *painter, 
                                 const QStyleOptionViewItem &option, 
                                 const QModelIndex &index, 
                                 bool active ) const
{
    QColor color =  index.data(NListView::FgColorRole).value<QColor>(); 
    if ( color.isValid( ))
    {
        painter->setPen(color);
    }
    else
    {
        painter->setPen(active? ACTIVE_COLOR:INACTIVE_COLOR);
    }

    QString caption = index.data(NListView::CaptionRole ).toString();
    QFontMetrics metrics(painter->font());
    
    painter->drawText(
                     QRect(option.rect.x() + CaptionPosition.x( ), 
                           option.rect.y( ) + CaptionPosition.y( ) +  option.rect.height( ) /2 - CaptionSize.height( )/2  , 
                           CaptionSize.width( ) , 
                           CaptionSize.height( ) ),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     metrics.elidedText(caption, Qt::ElideRight, CaptionSize.width(), Qt::TextShowMnemonic)
                     );

    drawCaptionHLine (painter, option, index, metrics.width(caption));
}

void NItemDelegate::drawSelection (QPainter *painter, 
                                   const QStyleOptionViewItem &option, 
                                   const QModelIndex &index) const
{
    if ( option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect.x() + CaptionPosition.x( ) - 10,
                          option.rect.y() + CaptionPosition.y() + ItemHintSize.height( ) / 2 - 4 ,
                          8, 8, QBrush(Qt::red));
    }
}


void NItemDelegate::drawIcon (QPainter *painter, 
                              const QStyleOptionViewItem &option, 
                              const QModelIndex &index) const
{
    if ( ShowIcon )
    {
        QString icon = index.data(NListView :: IconRole).toString();
        QPixmap pixmap;
        if ( !QPixmapCache :: find (icon , pixmap ) )
        {
            pixmap = QPixmap(icon);
            QPixmapCache :: insert ( icon , pixmap );
        }

        painter->drawPixmap(
                           option.rect.x() + LeftIconPosition.x( ) ,
                           option.rect.y()+LeftIconPosition.y( ) + ItemHintSize.height( )/2 - LeftIconSize.height( )/2,
                           LeftIconSize.width(),
                           LeftIconSize.height(),
                           pixmap
                           );
    }
}

void NItemDelegate::drawCaptionHLine (QPainter *painter, 
                                      const QStyleOptionViewItem &option, 
                                      const QModelIndex &index,
                                      int length) const
{
    int hline = index.data(NListView::HLineRole).toInt();
    if (hline)
    {
        painter->setPen(Qt::white);
        painter->drawLine(option.rect.x() + CaptionPosition.x(), 
                          option.rect.y() + CaptionPosition.y()  + option.rect.height( ) /2 ,
                          option.rect.x() + CaptionPosition.x() + length, 
                          option.rect.y() + CaptionPosition.y()  + option.rect.height( ) / 2);
    }
}

void NItemDelegate::computeLayout(QWidget *parent)
{
	QFontMetrics fm(parent->font());

	if (!ItemHintSize.isValid())
		ItemHintSize = QSize(parent->width() ,fm.height() * 5/3);

	if (!LeftArrowIconSize.isValid())
		LeftArrowIconSize = QSize(fm.height() * 13/30, fm.height() * 2/3);

	int border = ItemHintSize.height() / 5;

	if (!LeftIconSize.isValid())
		LeftIconSize = QSize(ItemHintSize.height() - border, ItemHintSize.height() - border);

	if (!RightIconSize.isValid())
		RightIconSize = LeftArrowIconSize;

	LeftArrowIconPosition = QPoint(border, 0);
	LeftIconPosition = QPoint(LeftArrowIconPosition.x() + LeftArrowIconSize.width() + border, 0);
	RightIconPosition = QPoint(parent->width() - RightIconSize.width() - border, 0);

	// Give a small border between left icon(s) and caption.
	//
	CaptionPosition = QPoint(border, 0);

	// Always count with left arrow icon.
	//
	if (hasShowIcon())
		CaptionPosition += QPoint(LeftIconPosition.x() + LeftIconSize.width(), 0);
	else
		CaptionPosition += QPoint(LeftArrowIconPosition.x() + LeftArrowIconSize.width(), 0);

	CaptionSize = QSize(parent->width() - CaptionPosition.x(), ItemHintSize.height());
	if (hasShowRightIcon())
		CaptionSize -= QSize(parent->width() - RightIconPosition.x(), 0);
}

