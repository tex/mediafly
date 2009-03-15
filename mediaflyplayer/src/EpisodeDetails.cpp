/*
 *  Copyright(C) 2009 Milan Svoboda. 
 *               <milan.svoboda@centrum.cz>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2 of the License.
 *  
 *
 *  This program is distributed in the hope that, in addition to its 
 *  original purpose to support Neuros hardware, it will be useful 
 *  otherwise, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 ****************************************************************************/

#include "EpisodeDetails.h"
#include "EpisodeModel.h"
#include <QDebug>
#include "nhelpbox.h"

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
	m_label->setText(m_index.data(mf::EpisodeModel::titleRole).toString());
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
		emit showPlayMenu(m_index);
		break;
	case Qt::Key_Left:
		if (mf::EpisodeModel::advanceToPreviousEpisode(m_index)) {
			update();
		}
		break;
	case Qt::Key_Right:
		if (mf::EpisodeModel::advanceToNextEpisode(m_index)) {
			update();
		}
		break;
	case Qt::Key_Escape:
		emit back();
		break;
	case Qt::Key_Help:
		NHelpBox::NHelpBoxNew(tr("Possible keys"),
		                      tr("Enter - Play current episode\n") +
		                      tr("Left - Show previous episode\n") +
		                      tr("Right - Show next episode\n") +
		                      tr("Back - Back to episode menu\n") +
		                      tr("Page Up/Down - Scroll with details information\n") +
		                      tr("Up/Down - Scroll with episode information\n"));
		break;
	case Qt::Key_PageUp:
	{
		if (QScrollBar* bar = m_details->verticalScrollBar())
			bar->triggerAction(QAbstractSlider::SliderPageStepSub);
		break;
	}
	case Qt::Key_PageDown:
	{
		if (QScrollBar* bar = m_details->verticalScrollBar())
			bar->triggerAction(QAbstractSlider::SliderPageStepAdd);
		break;
	}
	case Qt::Key_Up:
	{
		if (QScrollBar* bar = m_info->verticalScrollBar())
			bar->triggerAction(QAbstractSlider::SliderPageStepSub);
		break;
	}
	case Qt::Key_Down:
	{
		if (QScrollBar* bar = m_info->verticalScrollBar())
			bar->triggerAction(QAbstractSlider::SliderPageStepAdd);
		break;
	}
	default:
		event->ignore();
		break;
	}
}

