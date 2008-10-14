#ifndef MediaflyPlayAudio_H
#define MediaflyPlayAudio_H

#include "ui_MediaflyPlayAudio.h"
#include "MediaflyPlay.h"
#include "MediaflyEpisodeModel.h"

class MediaflyPlayAudio : public MediaflyPlay, private Ui::MediaflyPlayAudio
{
public:
	MediaflyPlayAudio(MediaflyEpisodeModel& episodeModel, QWidget *parent = 0);
};

#endif

