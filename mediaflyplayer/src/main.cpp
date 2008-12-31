#include "Player.h"
#include <QApplication>
#include <QFileInfo>

QString currentPath;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	QFileInfo info(argv[0]);
	currentPath = info.path();
	mf::Player v;
	v.show();
	return app.exec();
}

