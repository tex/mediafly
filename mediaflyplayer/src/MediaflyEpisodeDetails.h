#include "ui_MediaflyEpisodeDetails.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QModelIndex>
#include <QKeyEvent>

class MediaflyEpisodeDetails : public QWidget, private Ui::MediaflyEpisodeDetails
{
	Q_OBJECT
public:
	MediaflyEpisodeDetails(QWidget *parent = 0);
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

