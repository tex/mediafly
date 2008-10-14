#include "MediaflyPlayVideo.h"

MediaflyPlayVideo::MediaflyPlayVideo(MediaflyEpisodeModel& episodeModel, QWidget *parent) :
	MediaflyPlay(episodeModel, parent)
{
	Ui::MediaflyPlayVideo::setupUi(widget);
}

