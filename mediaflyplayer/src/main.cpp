#include "MediaflyPlayer.h"
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	MediaflyPlayer v;
	v.show();
	return app.exec();
}

