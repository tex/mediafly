#ifndef MediaflyPlay_H
#define MediaflyPlay_H

#include "ui_MediaflyPlay.h"
#include "MediaflyEpisodeModel.h"

class MediaflyPlay : public QWidget, protected Ui::MediaflyPlay
{
	Q_OBJECT

protected:
	MediaflyPlay(/*MediaflyEpisodeModel& episodeModel,*/ QWidget *parent = 0);
	virtual ~MediaflyPlay() { }

//	MediaflyEpisodeModel& m_episodeModel;
};

#endif

