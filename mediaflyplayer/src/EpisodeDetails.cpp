#include "EpisodeDetails.h"
#include "EpisodeModel.h"
#include <QDebug>

using namespace mf;

EpisodeDetails::EpisodeDetails(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
}

void EpisodeDetails::show(const QModelIndex& index)
{
	m_index = index;
	update();

	connect(m_index.model(), SIGNAL(refreshed()),
	        this, SLOT(updateImage()));
}

void EpisodeDetails::updateImage()
{
	if (m_icon->pixmap()->isNull()) {
		m_icon->setPixmap(m_index.data(mf::EpisodeModel::imageRole).value<QPixmap>());
	}
}

void EpisodeDetails::update()
{
	m_header->setText(tr("Episode Details"));
	m_label->setText(m_index.data(mf::EpisodeModel::titleRole).toString());
	m_length->setText("???");
	m_icon->setPixmap(m_index.data(mf::EpisodeModel::imageRole).value<QPixmap>());
	m_details->setHtml("<b>" + tr("Item details") + "</b><br><b>Link:</b><br>" +
	                  m_index.data(mf::EpisodeModel::urlRole).toString() +
	                  "<br><b>" + tr("Publication date:") + "</b><br>" +
	                  m_index.data(mf::EpisodeModel::publishedRole).toString() +
	                  "<br><b>" + tr("Enclosure:") + "</b><br>" +
	                  m_index.data(mf::EpisodeModel::urlOriginalRole).toString());
	m_info->setHtml(m_index.data(mf::EpisodeModel::descriptionRole).toString());
}

void EpisodeDetails::keyPressEvent(QKeyEvent *event)
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
		if (mf::EpisodeModel::advanceToNextEpisode(m_index)) {
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

