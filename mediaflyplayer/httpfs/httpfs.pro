######################################################################
# Automatically generated by qmake (2.01a) �t led 6 08:27:36 2009
######################################################################

TEMPLATE = app
TARGET = ../build/httpfs
DEPENDPATH += .
INCLUDEPATH +=	${TOOLCHAIN_USR_INSTALL}/include \
		${TOOLCHAIN_ARCH}/include \
		$PWD


MOC_DIR = ./build
UI_DIR = ./build
OBJECTS_DIR = ./build
INCLUDEPATH += ./build

DEFINES += USE_AUTH _FILE_OFFSET_BITS=64 _REENTRANT FUSE_USE_VERSION=25 _POSIX_C_SOURCE=200112L _POSIX_SOURCE _SVID_SOURCE _BSD_SOURCE _XOPEN_SOURCE=500
LIBS += -lfuse

# Input
SOURCES += httpfs.c