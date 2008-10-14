#ifndef MediaflyPlayVideo_H
#define MediaflyPlayVideo_H

#include "ui_MediaflyPlayVideo.h"
#include "MediaflyPlay.h"

class MediaflyPlayVideo : public MediaflyPlay, private Ui::MediaflyPlayVideo
{
public:
	MediaflyPlayVideo(QWidget *parent = 0);
};

#endif

