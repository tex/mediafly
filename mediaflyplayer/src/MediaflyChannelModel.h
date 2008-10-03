#ifndef MediaflyChannelModel_H
#define MediaflyChannelModel_H

#include "MediaflyChannelModelThread.h"
#include <QAbstractListModel>
#include <QStringList>

struct MediaflyChannelModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum Roles
	{
		nameRole = Qt::DisplayRole,
		slugRole = Qt::UserRole,
	};

	MediaflyChannelModel(QObject *parent = 0);
	MediaflyChannelModel(const MediaflyChannelModel &obj);
	~MediaflyChannelModel() { }

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &parent, int role) const;

public slots:
	void refresh();

signals:
	void refreshed();
	void error(const QString& errorMsg);

private:
	void readData();

	MediaflyChannelModelThread m_channelModelThread;

	QStringList m_nameList;
	QStringList m_slugList;

private slots:
	void handleError(const QString& errorMsg);
	void handleRefreshed(const MediaflyChannelModel& obj);

	friend class MediaflyChannelModelThread;
};

Q_DECLARE_METATYPE(MediaflyChannelModel);

#endif

