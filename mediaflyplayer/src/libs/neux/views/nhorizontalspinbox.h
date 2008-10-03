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
 * neux horizontal spinbox header.
 *
 * REVISION:
 * 
 * 1) Initial creation. ----------------------------------- 2007-11-14 CL
 *
 */
#ifndef NHORIZONTALSPINBOX_H
#define NHORIZONTALSPINBOX_H

#include <QtGui>

class NHorizontalSpinBox : public QWidget
{
    Q_OBJECT

public:
	enum DisplayRole
	{
		DR_STRING,
		DR_NUMBER
	};
	NHorizontalSpinBox(QWidget *parent = 0);
	void addItem(const QStringList &);
	void addItem(const QString &, const QVariant &userData = QVariant());
	void setShowLeftIcon(bool flag = true) { bShowLeftIcon = flag; }
	void setShowRightIcon(bool flag = true) { bShowRightIcon = flag; }
	void setIndex(int index);
	void setMaximumIndex(int max);
	void setMinimumIndex(int min);
	int  currentIndex() { return indexCount; }
	const QString currentText() { return stringList.at(indexCount); }
	int count() { return stringList.size(); }
	bool showLeftIcon() { return bShowLeftIcon; }
	bool showRightIcon() { return bShowRightIcon; }
	void removeItem(int index);
	QString itemText(int index);
	QVariant itemData();
	void setTextOption(const QTextOption &option);
	int findText(const QString &, Qt::MatchFlags flags = Qt::MatchExactly | Qt::MatchCaseSensitive); 
	void setLeftIconSize(const QSize &size);
	void setRightIconSize(const QSize &size);
	void setDisplayRole(const DisplayRole &);

	QSize sizeHint( ) const;

signals:
	void currentIndexChanged(int index);
	void currentIndexChanged(const QString &string);

protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);

private:
	void caculatePrivateRect();
	bool bShowLeftIcon;
	bool bShowRightIcon;
	int indexCount;
	DisplayRole role;
	int maxIndex;
	int minIndex;

	QSize leftIconSize;
	QSize rightIconSize;
	
	QRect editRect;
	QRect captionRect;
	QRect leftRect;
	QRect rightRect;
	
	QTextOption textOption;
	QVariant data;
	QStringList stringList;
	QEvent::Type focusType;

	/* For handling numeric key input  when spinbox is play a numeric role */
	enum NumericKeyInputState { InputIdle, Inputting };
	NumericKeyInputState inputState;
	QByteArray numberArray;

	bool hasMinValue;
	bool hasMaxValue;
};

#endif
