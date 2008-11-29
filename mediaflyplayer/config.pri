DEPENDPATH += $PWD/src
INCLUDEPATH +=	neux \
		/usr/include \
		/usr/local/include/xmms2 \
		$PWD/src

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wall
QT -= sql svg
QT += xml network
