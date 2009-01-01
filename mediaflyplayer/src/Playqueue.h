#ifndef Playqueue_H
#define Playqueue_H

#include "Play.h"
#include "ui_MediaflyPlayqueue.h"
#include "EpisodeModel.h"
#include "EpisodeDelegate.h"
#include "CheckResponseOk.h"
#include <QWidget>

namespace mf {

class Playqueue : public QWidget, private Ui::MediaflyPlayqueue
{
	Q_OBJECT
public:
	Playqueue(EpisodeModel& episodeModel, mf::Play *mediaflyPlay, QWidget *parent = 0);

signals:
	void back();

private:
	EpisodeModel             &m_episodeModel;
	mf::Play                 *m_mediaflyPlay;
	EpisodeDelegate          *m_itemDelegate;
	mf::CheckResponseOk       m_checkResponseOk;

	static const int itemsReadAtOnce = 10;

private slots:
	void uploadNextPartOfMenu();
	void updateEpisodeModel();
	void handleNowPlayingClicked();
	void handleRemoveButtonClicked();
	void handleCheckResponseOkDone();
	void handleStateChange();
};

}

#endif

