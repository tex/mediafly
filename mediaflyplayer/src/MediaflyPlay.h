#ifndef MediaflyPlay_H
#define MediaflyPlay_H

#include "ui_MediaflyPlay.h"
#include "MediaflyPlayAudio.h"
#include "MediaflyPlayVideo.h"
#include <QModelIndex>
#include <QKeyEvent>

class MediaflyPlay : public QWidget, protected Ui::MediaflyPlay
{
	Q_OBJECT

public:
	MediaflyPlay(QWidget *parent = 0);
	void show(const QModelIndex& index);

signals:
	void back();

private:
	QModelIndex m_index;

	void keyPressEvent(QKeyEvent *event);

//private slots:
//	void updateImage();
};

#endif

