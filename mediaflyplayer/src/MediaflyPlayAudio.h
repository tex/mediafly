#ifndef MediaflyPlayAudio_H
#define MediaflyPlayAudio_H

#include "ui_MediaflyPlayAudio.h"
#include "MediaflyPlay.h"

class MediaflyPlayAudio : public MediaflyPlay, private Ui::MediaflyPlayAudio
{
public:
	MediaflyPlayAudio(QWidget *parent = 0);
};

#endif

