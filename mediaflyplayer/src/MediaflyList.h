#include <QObject>
#include <QWidget>
#include <QListView>
#include <QKeyEvent>

class MediaflyList : public QListView
{
	Q_OBJECT
public:
	MediaflyList(QWidget *parent = 0);
	~MediaflyList();

signals:
	void almostAtEndOfList();
	void enterPressed();
	void leftPressed();
	void rightPressed();

private:
	void keyPressEvent(QKeyEvent *event);
};

