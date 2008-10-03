#include "MediaflyMenu.h"
#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	MediaflyMenu v;
	v.show();
	return app.exec();
}

