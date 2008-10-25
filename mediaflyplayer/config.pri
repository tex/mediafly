DEPENDPATH += $PWD/src
INCLUDEPATH +=	neux \
		$PWD/src

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wall
QT -= sql svg
QT += xml network
