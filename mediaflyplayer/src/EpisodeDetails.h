#ifndef mfEpisodeDetails_H
#define mfEpisodeDetails_H

#include "ui_MediaflyEpisodeDetails.h"
#include <QWidget>
#include <QModelIndex>
#include <QKeyEvent>

namespace mf {

class EpisodeDetails : public QWidget, private Ui::MediaflyEpisodeDetails
{
	Q_OBJECT
public:
	EpisodeDetails(QWidget *parent = 0);

	void show(const QModelIndex& index);

signals:
	void back();
	void showPlayMenu(const QModelIndex& index);

private:
	QModelIndex m_index;

	void keyPressEvent(QKeyEvent *event);
	void update();

private slots:
	void updateImage();
};

}

#endif

