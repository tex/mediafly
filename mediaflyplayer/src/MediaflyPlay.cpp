#include "MediaflyPlay.h"
#include "MediaflyEpisodeModel.h"

MediaflyPlay::MediaflyPlay(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(changeChannelButton, SIGNAL(clicked()),
	        this, SLOT(handleChannelButtonClicked()));
}

void MediaflyPlay::handleChannelsButtonClicked()
{
	emit backToChannelsMenu();
}

void MediaflyPlay::show(const QModelIndex& index)
{
	m_index = index;

	if (m_index.data(MediaflyEpisodeModel::formatRole).toString().compare("Video", Qt::CaseInsensitive) == 0)
	{
		video->show(m_index);
		stackedWidget->setCurrentWidget(video);
	}
	else
	{
		audio->show(m_index);
		stackedWidget->setCurrentWidget(audio);
	}

	setFocusPolicy(Qt::StrongFocus);
	setFocus();
}

void MediaflyPlay::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Escape:
		video->hide();
		audio->hide();
		emit back();
		break;
	default:
		event->ignore();
		break;
	}
}

