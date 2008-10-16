#include "MediaflyEpisodeDetails.h"
#include "MediaflyEpisodeModel.h"
#include <QDebug>

MediaflyEpisodeDetails::MediaflyEpisodeDetails(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
}

void MediaflyEpisodeDetails::show(const QModelIndex& index)
{
	m_index = index;
	update();

	connect(m_index.model(), SIGNAL(refreshed()),
	        this, SLOT(updateImage()));
}

void MediaflyEpisodeDetails::updateImage()
{
	if (m_icon->pixmap()->isNull()) {
		m_icon->setPixmap(m_index.data(MediaflyEpisodeModel::imageRole).value<QPixmap>());
	}
}

void MediaflyEpisodeDetails::update()
{
	m_header->setText("Episode Details");
	m_label->setText(m_index.data(MediaflyEpisodeModel::titleRole).toString());
	m_length->setText("???");
	m_icon->setPixmap(m_index.data(MediaflyEpisodeModel::imageRole).value<QPixmap>());
	m_details->setHtml("<b>Item details</b><br><b>Link:</b><br>" +
	                  m_index.data(MediaflyEpisodeModel::urlRole).toString() +
	                  "<br><b>Publication date:</b><br>" +
	                  m_index.data(MediaflyEpisodeModel::publishedRole).toString() +
	                  "<br><b>Enclosure:</b><br>" +
	                  m_index.data(MediaflyEpisodeModel::urlOriginalRole).toString());
	m_info->setHtml(m_index.data(MediaflyEpisodeModel::descriptionRole).toString());
}

void MediaflyEpisodeDetails::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Enter:
	case Qt::Key_Return:
	{
		emit showPlayMenu(m_index);
		break;
	}
	case Qt::Key_Left:
	{
		if (m_index.row() > 0) {
			m_index = m_index.model()->index(m_index.row() - 1, 0);
			update();
		}
		break;
	}
	case Qt::Key_Right:
	{
		if (MediaflyEpisodeModel::advanceToNextEpisode(m_index)) {
			update();
		}
		break;
	}
	case Qt::Key_Escape:
	{
		emit back();
		break;
	}
	default:
		event->ignore();
		break;
	}
}

