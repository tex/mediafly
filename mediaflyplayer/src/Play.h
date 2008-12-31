#ifndef MediaflyPlay_H
#define MediaflyPlay_H

#include "ui_MediaflyPlay.h"
#include "PlayAudio.h"
#include "PlayVideo.h"
#include <QModelIndex>
#include <QKeyEvent>

namespace mf {

class Play : public QWidget, protected Ui::MediaflyPlay
{
	Q_OBJECT
public:
	Play(QWidget *parent = 0);

	void show(const QModelIndex& index);
	void getState(QModelIndex &currentIndex, QString &songPosition, QString &songLength);

signals:
	void back();
	void backToChannelsMenu();
	void showPlayqueue();
	void stateChange();

private:
	enum State
	{
		STOP,
		PAUSE,
		PLAY,
	}
	m_state;

	QModelIndex m_index;

	void keyPressEvent(QKeyEvent *event);
	void update();
	void updateStateIndicator(enum State state);

private slots:
	void handleChannelsButtonClicked();
	void handleNextEpisodeButtonClicked();
	void handlePlayqueueButtonClicked();
};

}

#endif

