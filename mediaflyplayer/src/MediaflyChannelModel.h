#ifndef MediaflyChannelModel_H
#define MediaflyChannelModel_H

#include "Mediafly.h"
#include "MediaflyChannelModelData.h"
#include <QAbstractListModel>
#include <QList>

struct MediaflyChannelModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles
	{
		nameRole = Qt::DisplayRole,
		slugRole = Qt::UserRole + 1000,
	};

	MediaflyChannelModel(QObject *parent = 0);
	MediaflyChannelModel(const MediaflyChannelModel &obj);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

	void refresh();
	void cancel();

signals:
	void refreshed();

private:
	Mediafly                   *m_mediafly;
	MediaflyChannelModelData    m_modelData;
	QList<MediaflyChannelEntry> m_data;
	int                         m_id;

private slots:
	void handleEntryRead(const MediaflyChannelEntry& entry);
	void handleEntryFinished();
};

#endif

