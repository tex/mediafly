#include "MediaflyPlayAudio.h"

MediaflyPlayAudio::MediaflyPlayAudio(MediaflyEpisodeModel& episodeModel, QWidget *parent) :
	MediaflyPlay(episodeModel, parent)
{
	Ui::MediaflyPlayAudio::setupUi(widget);
}

