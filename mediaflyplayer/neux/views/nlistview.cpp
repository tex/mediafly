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
 * main-menu implement header.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-08 EY
 *
 */


#include <QtGui>


#include "nlistview.h"


NListView::NListView(QWidget *parent )
: QListView(parent) , itemMovePolicy(OverAndOver)
{
    setAttribute(Qt::WA_InputMethodEnabled,false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::NoSelection);
    setCaptionAnimationPolicy(AsNeeded);

	delegate = new NItemDelegate ( this );
	setItemDelegate ( delegate );

	scrollbar = new QScrollBar ( this );
	setVerticalScrollBar ( scrollbar );
	scrollbar->setVisible(true);

    setFrameStyle(QFrame::NoFrame );
 
    setupConnections ( );
}

NListView :: NListView ( QWidget *parent , ItemMovePolicy p )
	:QListView ( parent ) , itemMovePolicy(p)
{
    setAttribute(Qt::WA_InputMethodEnabled,false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::NoSelection);
    setCaptionAnimationPolicy(AsNeeded);
 
	delegate = new NItemDelegate ( this );
	setItemDelegate ( delegate );

	scrollbar = new QScrollBar ( this );
	setVerticalScrollBar ( scrollbar );
	scrollbar->setVisible(true);
	
    setupConnections ( );
}

NListView :: ~NListView ( )
{
	delete delegate;
	delete scrollbar;
}

void NListView :: setupConnections ( )
{
	connect ( delegate , SIGNAL(editorAboutToClosed(const QModelIndex &, const QString & )),
								this , SLOT(on_editor_about_to_closed( const QModelIndex & , const QString &)));
	connect ( delegate , SIGNAL(editorClosed(const QModelIndex &, const QString & ,bool)),
								this , SLOT(on_editor_closed( const QModelIndex & , const QString &,bool)));
}

void NListView :: setEditorText(const QString &text)
{
    delegate->setEditorText(text);
}

/* signal relay to upper application */
void NListView :: on_editor_about_to_closed ( const QModelIndex & index , 
                                              const QString & text  )
{
    emit editorAboutToClosed ( index , text );
}

void NListView :: on_editor_closed ( const QModelIndex & index , 
                                              const QString & text , bool result  )
{
    emit editorClosed( index , text , result);
}

void NListView :: on_editor_enable_set_data ( bool enable )
{
    delegate->onEnableEditorSetData( enable );
}

void NListView :: on_editor_enable_inputmethod ( bool enable )
{
    delegate->onEnableInputMethod(enable);
}

void NListView :: setItemDelegate( QAbstractItemDelegate *newDelegate )
{
	QListView :: setItemDelegate(newDelegate);
	delegate = dynamic_cast<NItemDelegate *>(newDelegate);
	delegate->computeLayout(dynamic_cast<QWidget*>(this));
}

void NListView :: resizeEvent ( QResizeEvent * e )
{
	delegate->computeLayout(dynamic_cast<QWidget*>(this));
	return QListView :: resizeEvent ( e );
}

QModelIndex NListView :: moveCursor ( CursorAction cursorAction, 
									  Qt::KeyboardModifiers modifiers )
{
	if (model() == 0)
		return QListView::moveCursor(cursorAction, modifiers);

    QModelIndex current = currentIndex();

	switch ( cursorAction )
	{
	case MoveUp:
         if (current.row() > 0)
            current = model()->index(current.row() - 1, 0,
                                     rootIndex());
        else
            current = model()->index(
                                    (itemMovePolicy == OverAndOver ? rows(current) - 1 : 0)
                                    , 0, rootIndex());

        if ( current.row( ) + 1 ==  ( rows( current ) - verticalScrollBar( )->maximum( ))/2 +
             verticalScrollBar()->value( ))
        {
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
        }

        emit currentItemMoved(current.row());
        break;
	case MoveDown:
         if (current.row() < rows(current) - 1)
            current = model()->index(current.row() + 1, 0,
                                     rootIndex());
        else
            current = model()->index(
                                    (itemMovePolicy == OverAndOver ? 0 : rows(current) - 1 )
                                    , 0 ,  rootIndex());

        if ( current .row( ) > (rows( current ) - verticalScrollBar( )->maximum( ))/2 + 
             verticalScrollBar()->value( )) 
        {
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
        }

        emit currentItemMoved(current.row());
        break;
	case MovePageUp:
         if ( currentIndex( ).row( ) > verticalScrollBar()->value( ) )
         {
             current = model()->index(verticalScrollBar()->value( ) ,
                                                0 , rootIndex( ) );
         }
         else if ( verticalScrollBar( )->value( ) > 0 )
         {
             verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
             current = model()->index(verticalScrollBar()->value( ) ,
                                                0 , rootIndex( ) );
         }
         emit currentItemMoved(current.row()); 
         break; 
	case MovePageDown:
         if ( currentIndex().row() + 1 < rows( currentIndex() ) - verticalScrollBar( )->maximum( )
              + verticalScrollBar()->value( ) )
         {
             current = model()->index(rows( currentIndex() ) - 
                                      verticalScrollBar( )->maximum( ) + 
                                      verticalScrollBar()->value( ) -1 ,
                                      0 , rootIndex( ) );
         }
         else if ( verticalScrollBar()->maximum( ) - verticalScrollBar()->value( ) > 0 )
         {
             verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
             current = model()->index(rows( currentIndex() ) - 
                                        verticalScrollBar( )->maximum( ) + 
                                        verticalScrollBar()->value( ) - 1,
                                        0 , rootIndex( ) );
         }
         emit currentItemMoved(current.row());
         break;
	case MoveNext:
        current = model()->index( 0 , 0, rootIndex());
        emit currentItemMoved(current.row());
        break;
	default:
		break;
	}

	return current;
}

int NListView::rows(const QModelIndex &index) const
{
    return model()->rowCount(model()->parent(index));
}

void NListView::keyPressEvent(QKeyEvent *event)
{
	event->ignore();
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Right) 
	{
		NItemDelegate* delegate = qobject_cast<NItemDelegate*>( itemDelegate(currentIndex()) );
		if (delegate != NULL)
		{
			delegate->setClickFlashActive(true);
			viewport()->repaint(); // request immediate repaint since we are animating
			clickFlashTimer.start(defClickFlashDuration, this);
		}
		else qDebug() << "WARNING: no delegate found when starting listview flash.";
	}
	return QListView::keyPressEvent(event);
}

void NListView::timerEvent(QTimerEvent *event)
{
	if (event->timerId() != clickFlashTimer.timerId()) return QWidget::timerEvent(event);
	endClickFlash();
}

void NListView :: endClickFlash()
{
	clickFlashTimer.stop();

	NItemDelegate* delegate = qobject_cast<NItemDelegate*>( itemDelegate(currentIndex()) );
	if (delegate != NULL) 
	{
		delegate->setClickFlashActive(false);
		viewport()->repaint(); // request immediate repaint since we are animating
	}
	else qDebug() << "WARNING: no delegate found when ending listview flash.";
}

void NListView :: setCaptionAnimationPolicy(ScrollAnimationPolicy policy)
{
    animationPolicy = policy;
    initScrollAnimation();
}

void NListView :: initScrollAnimation()
{
    switch (animationPolicy)
    {
    case AlwaysOff:
    default:
        bPaintAnimation = false;
        animation.stop();
        animation.setCurrentTime(0);
        disconnect(&animation, SIGNAL(frameChanged(int)), this, SLOT(onAnimationFrameChanged(int)));
        break;
    case AsNeeded:   
        bPaintAnimation = false;
        animation.stop();
        animation.setCurrentTime(0);
        animation.setDirection(QTimeLine::Forward);
        animation.setLoopCount (0); //loop forever
        animation.setUpdateInterval(defCaptionScrollInterval);
        connect(&animation, SIGNAL(frameChanged(int)), this, SLOT(onAnimationFrameChanged(int)));
        break;
    }
}

void NListView :: enableCaptionAnimation(const QModelIndex &index, bool enable)
{
    if (enable)
    {
        switch (animationPolicy)
        {
        case AlwaysOff:
        default:
            break;
        case AsNeeded:
            if (index.isValid() && (currentIndex() == index)) //For now, just design for scroll focus caption
            {
                QString caption = index.data(NListView::CaptionRole ).toString();
                QFontMetrics fm(viewport()->font());
                if (fm.width(caption) > delegate->captionSize().width())
                {
                    animationCaption = caption;
                    animationColor =  index.data(NListView::FgColorRole).value<QColor>(); 
                    if (!animationColor.isValid( ))
                    {
                        animationColor = QColor(ACTIVE_COLOR);
                    }
                    
                    int endFrame = animationCaption.size() - (delegate->captionSize().width() * 3 / 4) / fm.averageCharWidth();
                    animation.setFrameRange(0, endFrame > 0? endFrame:animationCaption.size());
                    animation.setDuration(animation.updateInterval() * (animation.endFrame() - animation.startFrame()) / defCaptionScrollStepFrame);
                    animation.start();
                    bPaintAnimation = false;
                }
            }
            break;
        }
    }
    else
    {
        bPaintAnimation = false;
        animation.stop();
        animation.setCurrentTime(0);
    }
}

void NListView :: onAnimationFrameChanged(int frame)
{
    bPaintAnimation = true;
    QRect currRect = visualRect(currentIndex());
    viewport()->update(QRect(captionOffset(), captionSize()).translated(currRect.left(), 
                                                                        currRect.top() + currRect.height() /2 
                                                                        - captionSize().height()/2));


}

void NListView :: focusInEvent (QFocusEvent *event)
{
    QListView :: focusInEvent(event);
    enableCaptionAnimation(currentIndex(), true);
}

void NListView :: focusOutEvent (QFocusEvent *event)
{
    enableCaptionAnimation(currentIndex(), false);

	if (clickFlashTimer.isActive()) endClickFlash();

    QListView :: focusOutEvent(event);
}

void NListView :: currentChanged (const QModelIndex &current, 
                                  const QModelIndex &previous)
{
    enableCaptionAnimation(previous, false);

	if (clickFlashTimer.isActive()) endClickFlash();

    QListView::currentChanged(current, previous);
    enableCaptionAnimation(current, true);
}


void NListView :: paintEvent ( QPaintEvent * e )
{
	if ( model() && model( )->rowCount( rootIndex( )) > 0 )
	{	
        
        if (bPaintAnimation && (QTimeLine::Running == animation.state()))
        {
            QPainter painter(viewport());
            QStyleOptionViewItem option = viewOptions();
            option.rect = visualRect(currentIndex());
            paintAnimation(&painter, option, e->rect());
            bPaintAnimation = false;
        }
        else
        {
            QListView :: paintEvent ( e );
        }
	}
	else
	{
		QPainter painter(viewport( ) );
		QStyleOptionViewItem option = viewOptions();
		option.state |= QStyle::State_HasFocus;
		option.rect.setSize( delegate->sizeHint(option,QModelIndex()));
		delegate->paint(&painter,option,QModelIndex());
		painter.drawText(QRect(option.rect.x() + delegate->leftIconOffset( ).x( ), 
                             option.rect.y( ) + delegate->leftIconOffset( ).y( ) +  
                             option.rect.height( ) /2 - delegate->captionSize( ).height( )/2 - 1 ,
                             delegate->captionSize( ).width( ) , delegate->captionSize( ).height( ) ), 
                         tr("No items to list") );
	}
}

void NListView :: paintAnimation(QPainter *painter, 
                                 const QStyleOptionViewItem &option,
                                 const QRect &rect) 
{
    switch (animationPolicy)
    {
    case AlwaysOff:
    default:
        break;
    case AsNeeded:
        {
            QPixmap pixmap = QPixmap(BACKGROUND_IMAGE).scaled(option.rect.size());
            painter->drawPixmap(rect, pixmap.copy(rect.translated(- option.rect.x(), - option.rect.y())));
            QFontMetrics fm(painter->font());
            QString caption = fm.elidedText(animationCaption.mid(animation.currentFrame()),
                                            Qt::ElideRight, rect.width(), Qt::TextShowMnemonic);
            painter->setPen(animationColor);
            painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, caption);
            delegate->drawCaptionHLine (painter, option, currentIndex(), fm.width(caption));
        }
        break;
    }
}

