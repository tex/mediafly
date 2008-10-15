#include "MediaflyPlay.h"
#include "MediaflyEpisodeModel.h"

MediaflyPlay::MediaflyPlay(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(changeChannelsButton, SIGNAL(clicked()),
	        this, SLOT(handleChannelsButtonClicked()));
	connect(nextEpisodeButton, SIGNAL(clicked()),
	        this, SLOT(handleNextEpisodeButtonClicked()));
}

void MediaflyPlay::handleChannelsButtonClicked()
{
	video->hide();
	audio->hide();
	emit backToChannelsMenu();
}

void MediaflyPlay::handleNextEpisodeButtonClicked()
{
	video->hide();
	audio->hide();
	MediaflyEpisodeModel::advanceToNextEpisode(m_index);
	update();
}

void MediaflyPlay::updateStateIndicator(enum State state)
{
	switch (state) {
	case STOP:
		playStateButton->setText("|");
		break;
	case PAUSE:
		playStateButton->setText("||");
		break;
	case PLAY:
		playStateButton->setText(">");
		break;
	default:
		Q_ASSERT(false);
	}
}

void MediaflyPlay::update()
{
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
}

void MediaflyPlay::show(const QModelIndex& index)
{
	m_index = index;

	update();

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

