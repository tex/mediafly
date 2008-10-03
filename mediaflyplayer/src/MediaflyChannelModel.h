#ifndef MediaflyChannelModel_H
#define MediaflyChannelModel_H

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

	void refresh();

private:
	QStringList m_nameList;
	QStringList m_slugList;
};

Q_DECLARE_METATYPE(MediaflyChannelModel);

#endif

