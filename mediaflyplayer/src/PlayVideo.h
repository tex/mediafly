#ifndef mfPlayVideo_H
#define mfPlayVideo_H

#include "ui_MediaflyPlayVideo.h"
#include <QModelIndex>

namespace mf {

class PlayVideo : public QWidget, private Ui::MediaflyPlayVideo
{
	Q_OBJECT
public:
	PlayVideo(QWidget *parent = 0);
	void show(const QModelIndex& index);
	void hide();

private:
	QModelIndex m_index;
};

}

#endif

