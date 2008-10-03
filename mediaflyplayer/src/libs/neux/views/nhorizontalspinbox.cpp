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
 * neux horizontal spinbox  implement routines.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-14 CL
 *
 */
#include "nhorizontalspinbox.h"
#include <QDebug>

const int defaultIconWidth = 10;
const int defaultCaptionLeftMargin = 5;
const int defaultCaptionRightMargin = 5;


NHorizontalSpinBox::NHorizontalSpinBox(QWidget *parent)
    : QWidget(parent), indexCount(0), role(DR_STRING),
	maxIndex(0), minIndex(0), hasMaxValue(false), hasMinValue(false) 
{
	setFocusPolicy(Qt::StrongFocus);
	QFontMetrics fm(font());
	leftIconSize = QSize(fm.height()*2/5, fm.height());
	rightIconSize = QSize(fm.height()*2/5, fm.height());

	setShowLeftIcon();
	setShowRightIcon();
	addItem(QStringList() << "NSpinEdit");
	textOption.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
}

void NHorizontalSpinBox::setDisplayRole(const DisplayRole &r)
{
	role = r;
	if (DR_NUMBER == role)
	{
		inputState = InputIdle;
	}
}

void NHorizontalSpinBox::removeItem(int index)
{
	if (index >= 0 && index < stringList.size())
	{
		stringList.removeAt(index);
	}
}

void NHorizontalSpinBox::setTextOption(const QTextOption &option) 
{ 
	textOption = option;
}

void NHorizontalSpinBox::focusInEvent(QFocusEvent *event)
{
	if (DR_NUMBER == role)
	{
		/* make the indexCount within the available range */
		if (hasMinValue && (indexCount < minIndex))
		{
			indexCount = minIndex;
		}
		else if (hasMaxValue && (indexCount > maxIndex))
		{
			indexCount = maxIndex;
		}

        /* initialise the number array for numeric spinbox */
		inputState = InputIdle;
		numberArray.setNum(indexCount);
	}

	focusType = QEvent::FocusIn;
	QSize size = sizeHint( );
	setMinimumSize(size);
	resize(size);
	update();
}

void NHorizontalSpinBox::setMaximumIndex(int max)
{
	hasMaxValue = true;
	maxIndex = max;
	if (hasMinValue && (maxIndex < minIndex))
	{
		minIndex = maxIndex;
	}
	
	if(indexCount > maxIndex)
		setIndex(maxIndex);
}

void NHorizontalSpinBox::setMinimumIndex(int min)
{
	hasMinValue = true;
	minIndex = min;
	if (hasMaxValue && (minIndex > maxIndex))
	{
		maxIndex = minIndex;
	}
	
	if(indexCount < minIndex)
		setIndex(minIndex);
}

void NHorizontalSpinBox::focusOutEvent(QFocusEvent *event)
{
	/* Check Index for numeric spinbox */
	if (DR_NUMBER == role)
	{
		if (Inputting ==  inputState)
		{
			int newValue = numberArray.toInt();
			if (hasMinValue && (newValue < minIndex))
			{
				newValue = minIndex;
			}
			else if (hasMaxValue && (newValue > maxIndex))
			{
				newValue = maxIndex;
			}

			if (newValue != indexCount)
			{
				indexCount = newValue;
				emit currentIndexChanged(indexCount);
			}

			numberArray.setNum(newValue);
			inputState = InputIdle;
		}
	}
	focusType = QEvent::FocusOut;
	QSize size = sizeHint( );
	setMinimumSize(size);
	resize(size);
	update( );
}

int NHorizontalSpinBox::findText(const QString &string, Qt::MatchFlags flags)
{
	return stringList.lastIndexOf(string);
}

void NHorizontalSpinBox::setLeftIconSize(const QSize &size) 
{ 
	if (!size.isNull())
	{
		leftIconSize = size;
	}
}

void NHorizontalSpinBox::setRightIconSize(const QSize &size) 
{ 
	if (!size.isNull())
	{
		rightIconSize = size; 
	}
}

QString NHorizontalSpinBox::itemText(int index)
{
	if (index >= 0 && index < stringList.size())
	{
		return stringList.at(index);
	}
	return QString();
}

QVariant NHorizontalSpinBox::itemData()
{
	if (data.isValid())
	{
		return data;
	}
	return QVariant();
}

void NHorizontalSpinBox::addItem(const QStringList &list)
{
	if (stringList.size() > 0)
		stringList.clear();
	stringList = list;
}

void NHorizontalSpinBox::addItem(const QString &string, const QVariant &userData)
{
	data = userData;
	if (stringList.size() > 0)
	{
		stringList.clear();
	}
	stringList.append(string);
}

  
void NHorizontalSpinBox::caculatePrivateRect()
{   
	leftRect = QRect(editRect.x(), editRect.y() + (editRect.height() - leftIconSize.height()) / 2, leftIconSize.width(), leftIconSize.height());

	QString indexString;
	if (role == DR_NUMBER)
		indexString = QString::number(InputIdle == inputState ? indexCount : numberArray.toInt());
	else if (stringList.size())
		indexString = stringList.at(indexCount);

	QFontMetrics fm(font()); // fetch the font which set by designer
	int strWidth = fm.width(indexString); // return 0 if the string is empty
	int rightIconX = editRect.x() + leftIconSize.width() + defaultCaptionLeftMargin +  strWidth + defaultCaptionRightMargin;
	if (rightIconX + rightIconSize.width() > editRect.x() + editRect.width()) //limit the max offset
		rightIconX = editRect.x() + editRect.width() - rightIconSize.width();
	rightRect = QRect(rightIconX, editRect.y() + (editRect.height() - rightIconSize.height()) / 2 , rightIconSize.width(), rightIconSize.height());

	captionRect = QRect(leftRect.x() + leftRect.width() + defaultCaptionLeftMargin, 
						editRect.y(), 
						rightRect.x() - leftRect.x() -  leftRect.width() - defaultCaptionRightMargin - defaultCaptionLeftMargin, 
						editRect.height());
}

void NHorizontalSpinBox::setIndex(int index) 
{ 
	if (role == DR_NUMBER)
	{
		if (!((hasMinValue && (index < minIndex)) || (hasMaxValue && (index > maxIndex))) && index != indexCount)
		{
			indexCount = index; 
			emit currentIndexChanged(indexCount);
		}
	}
	else
	{
		if ((index >= 0) && (index < stringList.size()) && (index != indexCount))
		{
			indexCount = index; 
			emit currentIndexChanged(indexCount);
			emit currentIndexChanged(stringList.at(indexCount));
		}
	}

	QSize size = sizeHint( );
	setMinimumSize(size);
	resize(size);
	update( );
}

void NHorizontalSpinBox::keyPressEvent(QKeyEvent *event)
{
	if (role == DR_NUMBER) //number role
	{
		switch (event->key())
		{
		case Qt::Key_Left:
			{
				numberArray.setNum(hasMinValue && (numberArray.toInt() - 1 < minIndex) ? maxIndex : numberArray.toInt() - 1);
				inputState = Inputting;
			}
			break;
		case Qt::Key_Right:
			{
				numberArray.setNum(hasMaxValue && (numberArray.toInt() + 1 > maxIndex) ? minIndex : numberArray.toInt() + 1);
				inputState = Inputting;
			}
			break;
		case Qt::Key_Clear:
			{
				if (!numberArray.isEmpty())
				{
					numberArray.remove(numberArray.size() - 1, 1).toInt();
				}
                inputState = Inputting;
			}
			break;
		case Qt::Key_0:
		case Qt::Key_1:
		case Qt::Key_2:
		case Qt::Key_3:
		case Qt::Key_4:
		case Qt::Key_5:
		case Qt::Key_6:
		case Qt::Key_7:
		case Qt::Key_8:
		case Qt::Key_9:
			{
				if (InputIdle == inputState)
				{
					numberArray.setNum(event->key() - Qt::Key_0).toInt();
					inputState = Inputting;
				}
				else if (Inputting == inputState)
				{
					numberArray.append(QChar(0x30 + event->key() - Qt::Key_0).toAscii()).toInt(); //0x30: Ascii code of '0' 
				}

				if (hasMaxValue && (numberArray.toInt() > maxIndex))
				{
					numberArray.setNum(maxIndex);
				}
			}
			break;
		default:
			break;
		}

		int currInput = numberArray.toInt();
		if (!((hasMinValue && (currInput < minIndex)) || (hasMaxValue && (currInput > maxIndex))) && (currInput != indexCount))
		{
			indexCount = currInput;
			emit currentIndexChanged(currInput);
		}
	}
	else if (role == DR_STRING)	// string role
	{
		switch (event->key())
		{
		case Qt::Key_Left:
			{
				if (indexCount > 0)
					indexCount--;
				else
					indexCount = stringList.size() - 1;
				emit currentIndexChanged(indexCount);
				emit currentIndexChanged(stringList.at(indexCount));
			}
			break;
		case Qt::Key_Right:
			{
				if (indexCount < stringList.size() - 1)
					indexCount++;
				else
					indexCount = 0;
				emit currentIndexChanged(indexCount);
				emit currentIndexChanged(stringList.at(indexCount));
			}
			break;
		default:
			break;
		}
	}

	QSize size = sizeHint( );
	setMinimumSize(size);
	resize(size);
	update( );

	event->ignore();
	QWidget::keyPressEvent(event);
}

void NHorizontalSpinBox::paintEvent(QPaintEvent *event)
{
    caculatePrivateRect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

	QString indexString;
	if (role == DR_NUMBER)
	{
		indexString = QString::number(InputIdle == inputState ? indexCount : numberArray.toInt());
	}
	else
	{
		if (stringList.size())
			indexString = stringList.at(indexCount);
	}

	painter.drawText(captionRect,
					 QFontMetrics(font()).elidedText(indexString,Qt::ElideRight, captionRect.width() ,Qt::TextShowMnemonic), 
					 textOption);

	if (focusType == QEvent::FocusIn)
	{
		QStyleOption option;
		if (bShowLeftIcon)
		{
            option.initFrom(this);
			option.rect = leftRect;
			style()->drawPrimitive(QStyle::PE_IndicatorArrowLeft, &option, &painter);
		}
			
		if (bShowRightIcon)
		{
			option.initFrom(this);
			option.rect = rightRect;
			style()->drawPrimitive(QStyle::PE_IndicatorArrowRight, &option, &painter);
		}
	}
}

void NHorizontalSpinBox::resizeEvent(QResizeEvent * event )
{
	QWidget::resizeEvent(event);
	editRect.setWidth(event->size().width());
	editRect.setHeight(event->size().height());
}

QSize NHorizontalSpinBox :: sizeHint( ) const
{
	QString text;
	if (role == DR_NUMBER)
		text = QString::number(InputIdle == inputState ? indexCount : numberArray.toInt());
	else if (stringList.size())
		text = stringList.at(indexCount);

	QFontMetrics fm(font( ));
	int preferW = fm.width(text) + leftIconSize.width( ) + rightIconSize.width( ) + defaultCaptionLeftMargin + defaultCaptionRightMargin;
	int preferH = qMax(fm.height( ), qMax(leftIconSize.height( ), rightIconSize.height( )));
	preferH = qMax(preferH, height( ));

	preferW = qBound(minimumWidth( ), preferW, maximumWidth( ));
	preferH = qBound(minimumHeight( ), preferH, maximumHeight( ));

	return QSize (preferW, preferH);
}


