#ifndef mfPlayAudio_H
#define mfPlayAudio_H

#include "ui_MediaflyPlayAudio.h"
#include <QModelIndex>

namespace mf {

class PlayAudio : public QWidget, private Ui::MediaflyPlayAudio
{
	Q_OBJECT
public:
	PlayAudio(QWidget *parent = 0);
	void show(const QModelIndex& index);
	void hide();

private:
	QModelIndex m_index;

private slots:
	void updateImage();
};

}

#endif

