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
 * NListView implement header.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-08 EY
 *
 */

#ifndef NLISTVIEW_H
#define NLISTVIEW_H


#include <QAbstractItemView>
#include <QFont>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QWidget>
#include <QListView>
#include <QScrollBar>
#include <QBasicTimer>
#include "nitemdelegate.h"
  
class NListView : public QListView
{
    Q_OBJECT



    public:
    enum Roles
    {
        CaptionRole = Qt::DisplayRole,
        IconRole = Qt::UserRole + 1,
        RightIconRole ,
		FgColorRole,
		HLineRole,
    };
    enum ItemMovePolicy
    {
        TopOrBottom,
        OverAndOver,
    };

    enum ScrollAnimationPolicy
    {
        AlwaysOff,
        AsNeeded
    };

    explicit NListView(QWidget *parent = 0);
	NListView(QWidget *parent ,ItemMovePolicy p );

	~NListView ( );

     int getCurrentRow() const
    {
        return currentIndex().row();
    }
    void setCurrentRow(int r)
    {
        setCurrentIndex(model()->index(r, 0,rootIndex()));
    }

    void setItemDelegate(QAbstractItemDelegate *newDelegate);

    void setShowLeftArrowIcon ( bool b ) { delegate->setShowLeftArrowIcon( b );}
    void setShowRightIcon(bool b){ delegate->setShowRightIcon( b ) ;}
     void setShowIcon(bool b) { delegate->setShowIcon( b );}
    bool hasShowIcon() const{ return delegate->hasShowIcon( );}
    bool hasShowRightIcon() const { return delegate->hasShowRightIcon( );}
    bool hasShowLeftArrowIcon ( ) const { return delegate->hasShowLeftArrowIcon( );}

    void setLeftArrowIconOffset ( const QPoint & point ){ delegate->setLeftArrowIconOffset(point);}
    void setLeftIconOffset ( const QPoint &point ){ delegate->setLeftIconOffset(point);}
    void setRightIconOffset ( const QPoint & point ){delegate->setRightIconOffset(point);}
    void setCaptionOffset ( const QPoint &point ){delegate->setCaptionOffset(point);}
    void setItemHintSize ( const QSize &size ) { delegate->setItemHintSize(size);}
    void setLeftArrowIconSize ( const QSize &size ) { delegate->setLeftArrowIconSize(size);}
    void setLeftIconSize ( const QSize &size ){ delegate->setLeftIconSize(size);}
    void setRightIconSize ( const QSize & size ){delegate->setRightIconSize(size);}
    void setCaptionSize ( const QSize &size ){delegate->setCaptionSize(size);}

    const QSize itemHintSize( ) const { return delegate->itemHintSize( );}
    const QSize leftArrowIconSize ( ) const {return delegate->leftArrowIconSize( );}
    const QSize leftIconSize ( ) const { return delegate->leftIconSize( ); }
    const QSize rightIconSize ( ) const {return delegate->rightIconSize( );}
    const QSize captionSize ( ) const {return delegate->captionSize( );}

    const QPoint leftArrowIconOffset ( ) const { return delegate->leftArrowIconOffset( );}
    const QPoint rightIconOffset ( ) const {return delegate->rightIconOffset( );}
    const QPoint captionOffset ( ) const{return delegate->captionOffset( );}
    const QPoint leftIconOffset ( ) const { return delegate->leftIconOffset( );}

    void setCaptionAnimationPolicy(ScrollAnimationPolicy policy);


public	slots:
	void on_editor_closed( const QModelIndex & index , const QString &text ,bool result);
    void on_editor_about_to_closed ( const QModelIndex & index , const QString &text );
    void on_editor_enable_set_data ( bool enable );
    void on_editor_enable_inputmethod ( bool enable );
    void setEditorText(const QString &text);

signals: 
    void currentItemMoved(int row);

    void editorAboutToClosed( const QModelIndex &index , const QString &text);
    void editorClosed( const QModelIndex &index , const QString &text , bool result);

protected:
    void focusInEvent (QFocusEvent *event);
    void focusOutEvent (QFocusEvent *event);
    void currentChanged (const QModelIndex &current, const QModelIndex &previous);
    void keyPressEvent(QKeyEvent *event);
	void resizeEvent ( QResizeEvent *);
	void paintEvent ( QPaintEvent * );
    QModelIndex moveCursor ( CursorAction cursorAction, 
							 Qt::KeyboardModifiers modifiers );


	void paintAnimation(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect);
	ScrollAnimationPolicy animationPolicy;
	QTimeLine animation;
	QString animationCaption;
	QColor animationColor;
	bool bPaintAnimation;

	QBasicTimer clickFlashTimer;
	void timerEvent(QTimerEvent *event);
	void endClickFlash();

	static const int defCaptionScrollInterval = 500; // msec
	static const int defCaptionScrollStepFrame = 1;
	static const int defClickFlashDuration = 150; // msec

    ItemMovePolicy itemMovePolicy;

protected slots:
    virtual void onAnimationFrameChanged(int frame);

private:
    void setupConnections( );
    int rows(const QModelIndex &index) const;
    void initScrollAnimation();
    void enableCaptionAnimation(const QModelIndex &index, bool enable);

private:
	QScrollBar *scrollbar;

protected:
    NItemDelegate *delegate;


};


#endif // NLISTVIEW_H

