#include "MediaflyEpisodeDetailsView.h"
#include "MediaflyEpisodeModel.h"
#include <QDebug>

MediaflyEpisodeDetailsView::MediaflyEpisodeDetailsView(QModelIndex& index) :
	QWidget(),
	m_index (index)
{
	setFocusPolicy(Qt::StrongFocus);
	setFocus();

	m_header.setAlignment(Qt::AlignRight);
	m_length.setAlignment(Qt::AlignRight);

	m_details.setReadOnly(true);
	m_info.setReadOnly(true);

	m_hLayoutName.addWidget(&m_label);
	m_hLayoutName.addWidget(&m_length);

	m_hLayoutDetails.addWidget(&m_icon);
	m_hLayoutDetails.addWidget(&m_details);

	m_vLayout.addWidget(&m_header);
	m_vLayout.addLayout(&m_hLayoutName);
	m_vLayout.addLayout(&m_hLayoutDetails);
	m_vLayout.addWidget(&m_info);

	setLayout(&m_vLayout);

	connect(m_index.model(), SIGNAL(refreshed()),
	        this, SLOT(updateImage()));

	update();
}

void MediaflyEpisodeDetailsView::updateImage()
{
	if (m_icon.pixmap()->isNull()) {
		QPixmap icon; icon.loadFromData(m_index.data(MediaflyEpisodeModel::imageRole).toByteArray());
		m_icon.setPixmap(icon);
	}
}

void MediaflyEpisodeDetailsView::update()
{
	m_header.setText("Episode Details");
	m_label.setText(m_index.data(MediaflyEpisodeModel::titleRole).toString());
	m_length.setText("???");
	QPixmap icon; icon.loadFromData(m_index.data(MediaflyEpisodeModel::imageRole).toByteArray());
	m_icon.setPixmap(icon);
	m_details.setHtml("<b>Item details</b><br><b>Link:</b><br>" +
	                  m_index.data(MediaflyEpisodeModel::urlRole).toString() +
	                  "<br><b>Publication date:</b><br>" +
	                  m_index.data(MediaflyEpisodeModel::publishedRole).toString() +
	                  "<br><b>Enclosure:</b><br>" +
	                  m_index.data(MediaflyEpisodeModel::urlOriginalRole).toString());
	m_info.setHtml(m_index.data(MediaflyEpisodeModel::descriptionRole).toString());
}

void MediaflyEpisodeDetailsView::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Left:
	{
		if (m_index.row() > 0)
			m_index = m_index.model()->index(m_index.row() - 1, 0);
		break;
	}
	case Qt::Key_Right:
	{
		if (m_index.row() + 15 > m_index.model()->rowCount())
			dynamic_cast<MediaflyEpisodeModel*>(const_cast<QAbstractItemModel*>(m_index.model()))->refresh();
		if (m_index.row() + 1 < m_index.model()->rowCount())
			m_index = m_index.model()->index(m_index.row() + 1, 0);
		break;
	}
	default:
		event->ignore();
		return;
	}
	update();
}

