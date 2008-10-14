#ifndef MediaflyPlayVideo_H
#define MediaflyPlayVideo_H

#include "ui_MediaflyPlayVideo.h"
#include <QModelIndex>

class MediaflyPlayVideo : public QWidget, private Ui::MediaflyPlayVideo
{
	Q_OBJECT
public:
	MediaflyPlayVideo(QWidget *parent = 0);
	void show(const QModelIndex& index);
	void hide();

private:
	QModelIndex m_index;
};

#endif

