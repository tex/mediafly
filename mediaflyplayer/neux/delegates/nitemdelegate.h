#ifndef _NITEMDELEGATE_H__
#define _NITEMDELEGATE_H__
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
 * neux NListView delegate header.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-12-14 SZ
 *
 */
#include <QItemDelegate>
#include <QtGui>

#define BACKGROUND_IMAGE ":/neux/CursorBar.png"
#define BACKGROUND_IMAGE_FLASH ":/neux/CursorBarFlash.png"
#define LEFT_ARROW  ":/neux/grey-arrow-left.gif"
#define INACTIVE_COLOR   QColor(0x64,0x64,0x64,0xff)
#define ACTIVE_COLOR       Qt::white



class NItemDelegate : public QItemDelegate
{
     Q_OBJECT

public slots:
    void onEnableEditorSetData ( bool enable );
     void onEnableInputMethod ( bool enable );
     void setEditorText(const QString &text);

signals:
    void editorAboutToClosed( const QModelIndex & index , const QString &text ) const;
    void editorClosed( const QModelIndex & index , const QString &text ,bool result) const;
 public:
    NItemDelegate(QWidget *parent = 0) ;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                        const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
						const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;

    QSize sizeHint ( const QStyleOptionViewItem & option, 
											const QModelIndex & index ) const  ;

    void setShowIcon(bool b) { ShowIcon = b; }
    void setShowRightIcon(bool b) { ShowRightIcon = b ;}
    void setShowLeftArrowIcon ( bool b ) { ShowLeftArrowIcon = b ;}
    bool hasShowRightIcon() const { return ShowRightIcon; }
    bool hasShowIcon() const { return ShowIcon; }
    bool hasShowLeftArrowIcon ( ) const { return ShowLeftArrowIcon;}

    const QSize itemHintSize( ) const { return ItemHintSize;}
    const QSize leftArrowIconSize ( ) const {return LeftArrowIconSize;}
    const QSize leftIconSize ( ) const { return LeftIconSize; }
    const QSize rightIconSize ( ) const {return RightIconSize;}
    const QSize captionSize ( ) const {return CaptionSize;}

    const QPoint leftArrowIconOffset ( ) const { return LeftArrowIconPosition;}
    const QPoint rightIconOffset ( ) const {return RightIconPosition;}
    const QPoint captionOffset ( ) const{return CaptionPosition;}
    const QPoint leftIconOffset ( ) const { return LeftIconPosition;}

    void setLeftArrowIconOffset ( const QPoint & point ){ LeftArrowIconPosition = point; }
    void setLeftIconOffset ( const QPoint &point ){ LeftIconPosition = point;}
    void setRightIconOffset ( const QPoint & position ){RightIconPosition = position;}
    void setCaptionOffset ( const QPoint &point ){CaptionPosition = point;}
    void setItemHintSize ( const QSize &hintSize ) { ItemHintSize = hintSize; }
    void setLeftArrowIconSize ( const QSize &size ) { LeftArrowIconSize = size;}
    void setLeftIconSize ( const QSize &size ){ LeftIconSize = size;}
    void setRightIconSize ( const QSize & size ){RightIconSize = size;}
    void setCaptionSize ( const QSize &size ){CaptionSize = size;}
    void setClickFlashActive (bool isActive) { isClickFlashActive = isActive; }

    virtual void drawFocus (QPainter *painter, const QStyleOptionViewItem &option) const;
    virtual void drawLeftArrowIcon (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void drawRightIcon (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void drawDisplay (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, bool active) const;
    virtual void drawSelection (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void drawIcon (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void drawCaptionHLine (QPainter *painter, const QStyleOptionViewItem &option,  const QModelIndex &index, int length) const;

protected:
    QSize LeftArrowIconSize;
    mutable QSize ItemHintSize;
    QSize LeftIconSize;
    QSize RightIconSize;
    QSize CaptionSize;

    QPoint LeftIconPosition;
    QPoint LeftArrowIconPosition;
    QPoint RightIconPosition;
    QPoint CaptionPosition;

     bool ShowIcon;
	 bool ShowRightIcon;
     bool ShowLeftArrowIcon;
     bool isClickFlashActive;
     mutable bool isSetData;
     mutable QWidget *curEditor;
     mutable QModelIndex focusIndex;

    QPainterPath prepareSelectionPath (const QRect& itemRect) const;
};

#endif
