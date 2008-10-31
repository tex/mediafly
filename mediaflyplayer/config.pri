DEPENDPATH += $PWD/src
INCLUDEPATH +=	${TOOLCHAIN_USR_INSTALL}/include \
		${TOOLCHAIN_USR_INSTALL}/include/neux \
		${TOOLCHAIN_USR_INSTALL}/include/xmms2 \
		${TOOLCHAIN_USR_INSTALL}/include/Neuros-Cooler/media \
		${TOOLCHAIN_USR_INSTALL}/include/Neuros-Cooler/core \
		${TOOLCHAIN_ARCH}/include \
		$PWD/src

LIBS += -L${TOOLCHAIN_USR_INSTALL}/lib
rootfs.path = /${ROOTFS}/fs/usr/local/bin

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wall
QT -= sql svg
QT += xml network
