#ifndef MediaflyPlayAudio_H
#define MediaflyPlayAudio_H

#include "ui_MediaflyPlayAudio.h"
#include <QModelIndex>

class MediaflyPlayAudio : public QWidget, private Ui::MediaflyPlayAudio
{
	Q_OBJECT
public:
	MediaflyPlayAudio(QWidget *parent = 0);
	void show(const QModelIndex& index);
	void hide();

private:
	QModelIndex m_index;
};

#endif

