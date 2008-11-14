#ifndef Playqueue_H
#define Playqueue_H

#include "ui_MediaflyPlayqueue.h"
#include "MediaflyEpisodeModel.h"
#include "MediaflyEpisodeDelegate.h"
#include "MediaflyCheckResponseOk.h"
#include <QWidget>

namespace mf {

class Playqueue : public QWidget, private Ui::MediaflyPlayqueue
{
	Q_OBJECT
public:
	Playqueue(MediaflyEpisodeModel& episodeModel, QWidget *parent = 0);

signals:
	void back();

private:
	MediaflyEpisodeModel     &m_episodeModel;
	MediaflyEpisodeDelegate  *m_itemDelegate;
	MediaflyCheckResponseOk   m_checkResponseOk;

	static const int itemsReadAtOnce = 10;

private slots:
	void uploadNextPartOfMenu();
	void updateEpisodeModel();
	void handleNowPlayingClicked();
	void handleRemoveButtonClicked();
	void handleCheckResponseOkDone();
};

}

#endif

