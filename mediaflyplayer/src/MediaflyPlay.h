#ifndef MediaflyPlay_H
#define MediaflyPlay_H

#include "ui_MediaflyPlay.h"
#include "MediaflyPlayAudio.h"
#include "MediaflyPlayVideo.h"
#include <QModelIndex>
#include <QKeyEvent>

class MediaflyPlay : public QWidget, protected Ui::MediaflyPlay
{
	Q_OBJECT
public:
	MediaflyPlay(QWidget *parent = 0);
	void show(const QModelIndex& index);

signals:
	void back();
	void backToChannelsMenu();

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
};

#endif

