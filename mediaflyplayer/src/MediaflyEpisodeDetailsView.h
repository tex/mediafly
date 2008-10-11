#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QModelIndex>
#include <QKeyEvent>

class MediaflyEpisodeDetailsView : public QWidget
{
	Q_OBJECT
public:
	MediaflyEpisodeDetailsView(QModelIndex& index);

private:
	QHBoxLayout      m_hLayoutDetails;
	QHBoxLayout      m_hLayoutName;
	QVBoxLayout      m_vLayout;

	QLabel m_header;
	QLabel m_icon;
	QLabel m_label;
	QLabel m_length;
	QTextEdit m_details;
	QTextEdit m_info;

	QModelIndex m_index;
	void keyPressEvent(QKeyEvent *event);

	void update();

private slots:
	void updateImage();
};

