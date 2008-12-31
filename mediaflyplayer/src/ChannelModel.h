#ifndef mfChannelModel_H
#define mfChannelModel_H

#include "Mediafly.h"
#include "ChannelModelData.h"
#include <QAbstractListModel>
#include <QList>

namespace mf {

struct ChannelModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles
	{
		nameRole = Qt::DisplayRole,
		slugRole = Qt::UserRole + 1000,
	};

	ChannelModel(QObject *parent = 0);
	ChannelModel(const ChannelModel &obj);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

	void refresh();
	void cancel();

signals:
	void refreshed();

private:
	Mediafly                   *m_mediafly;
	mf::ChannelModelData        m_modelData;
	QList<MediaflyChannelEntry> m_data;
	int                         m_id;

private slots:
	void handleEntryRead(const MediaflyChannelEntry& entry);
	void handleEntryFinished();
};

}

#endif

