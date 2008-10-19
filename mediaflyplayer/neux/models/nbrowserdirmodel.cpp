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
 * FileBrowser model  implement routines.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-12-03 SZ
 * 2) set caption to return utf8. ------------------------- 2008-03-06 JChen
 *
 */
#include "nbrowserdirmodel.h"
#include "nlistview.h"
#include "supportedmediatypes.h"
#include <QTextCodec>

NBrowserDirModel :: NBrowserDirModel ( QObject *parent )
	: QDirModel ( parent )
{

}

QVariant NBrowserDirModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 )
        return QVariant();

	if (role == NListView::CaptionRole )
    {
	    /* Display multi-language files name in play.browser */
	    QTextCodec *codec = QTextCodec::codecForName("UTF-8") ;
	            return codec->toUnicode(QDirModel :: data(index).toString( ).toUtf8( )); 
       // return QDirModel :: data( index );
    }
    if (role == NListView::IconRole )
    {
		if ( isDir( index ) )
			return QString(":/neux/folder.gif");

		SupportedMediaTypes type;
		switch ( type.mediaType( QFileInfo(index.data( ).toString( )).suffix( ) ) )
		{
		case SupportedMediaTypes::XmmsAudio:
		case SupportedMediaTypes::Audio:
			return QString(":/neux/Audio_v2.png");
			break;
		case SupportedMediaTypes::Video:
			return QString(":/neux/video_v2.png");
            break;
		case SupportedMediaTypes::Firmware:

			break;
		case SupportedMediaTypes::NotSupported:
			return QString(":/neux/UnkownFiletype.png");
			break;
		case SupportedMediaTypes::Image:
			return QString(":/neux/image_v2.png");
			break;
		default:
			break;
		}
    }
    if (role == NListView::RightIconRole )
		return QString(":/neux/navright.gif");
	

    return QVariant();
}



