#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QModelIndex>
#include <QKeyEvent>

class MediaflyEpisodeDetails : public QWidget
{
	Q_OBJECT
public:
	MediaflyEpisodeDetails(QWidget *parent = 0);
	void show(const QModelIndex& index);

signals:
	void back();
	void showPlayMenu(const QModelIndex& index);

private:
	QHBoxLayout     *m_hLayoutDetails;
	QHBoxLayout     *m_hLayoutName;
	QVBoxLayout     *m_vLayout;

	QLabel    *m_header;
	QLabel    *m_icon;
	QLabel    *m_label;
	QLabel    *m_length;
	QTextEdit *m_details;
	QTextEdit *m_info;

	QModelIndex m_index;
	void keyPressEvent(QKeyEvent *event);

	void update();

private slots:
	void updateImage();
};

