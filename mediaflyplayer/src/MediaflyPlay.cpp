#include "MediaflyPlay.h"
#include "MediaflyEpisodeModel.h"

MediaflyPlay::MediaflyPlay(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	connect(m_changeChannelsButton, SIGNAL(clicked()),
	        this, SLOT(handleChannelsButtonClicked()));
	connect(m_nextEpisodeButton, SIGNAL(clicked()),
	        this, SLOT(handleNextEpisodeButtonClicked()));
	connect(m_playqueueButton, SIGNAL(clicked()),
	        this, SLOT(handlePlayqueueButtonClicked()));
}

void MediaflyPlay::handleChannelsButtonClicked()
{
	m_video->hide();
	m_audio->hide();
	emit backToChannelsMenu();
}

void MediaflyPlay::handleNextEpisodeButtonClicked()
{
	if (MediaflyEpisodeModel::advanceToNextEpisode(m_index))
	{
		update();
	}
}

void MediaflyPlay::handlePlayqueueButtonClicked()
{
	m_video->hide();
	m_audio->hide();
	emit showPlayqueue();
}

void MediaflyPlay::updateStateIndicator(enum State state)
{
	switch (state) {
	case STOP:
		m_playStateButton->setText("|");
		break;
	case PAUSE:
		m_playStateButton->setText("||");
		break;
	case PLAY:
		m_playStateButton->setText(">");
		break;
	default:
		Q_ASSERT(false);
	}
}

void MediaflyPlay::update()
{
	QString format = m_index.data(MediaflyEpisodeModel::formatRole).toString();
	if (format.startsWith("Video", Qt::CaseInsensitive) == 0)
	{
		m_video->show(m_index);
		m_stackedWidget->setCurrentWidget(m_video);
	}
	else
	{
		m_audio->show(m_index);
		m_stackedWidget->setCurrentWidget(m_audio);
	}
	emit stateChange();
}

void MediaflyPlay::show(const QModelIndex& index)
{
	m_index = index;

	update();
}

void MediaflyPlay::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Escape:
		m_video->hide();
		m_audio->hide();
		emit back();
		break;
	default:
		event->ignore();
		break;
	}
}

void MediaflyPlay::getState(QModelIndex &currentIndex, QString &songPosition, QString &songLength)
{
	currentIndex = m_index;

	// TODO
	// songPosition = ...
	// songLength = ...
}

