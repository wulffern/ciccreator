######################################################################
# Automatically generated by qmake (2.01a) Tue Jul 16 19:48:51 2013
######################################################################

QT += core widgets


unix:!mac{
   QMAKE_LIBDIR += ~/qt5_4/lib  ~/qt5_4/plugins/platforms
}


CONFIG += qt console debug

mac {
  CONFIG -= app_bundle
}

TEMPLATE = app

osx:DESTDIR = bin/darwin
linux:DESTDIR = bin/linux

TARGET = cic

OBJECTS_DIR=build
MOC_DIR=build

DEPENDPATH += .

INCLUDEPATH += src/ external/libgds_dist/

# Input

HEADERS +=  src/core/rules.h  \
src/core/rect.h \
src/core/cell.h \
src/core/design.h \ 
src/core/patterntile.h \
src/core/patterntransistor.h \
src/core/patterncapacitor.h \
src/printer/designprinter.h \
src/printer/svg.h \
src/spice/spiceparser.h\
src/spice/spiceobject.h \
src/spice/subckt.h \
src/spice/subcktinstance.h \
    src/core/instance.h \
    src/core/layoutcell.h \
    src/gui/window.h \
    src/gui/renderarea.h \
    src/core/consoleoutput.h \
    external/libgds_dist/libgds.h \
    src/printer/gds.h


SOURCES +=     src/core/rules.cpp src/core/rect.cpp src/core/cell.cpp  src/main.cpp \ 
    src/core/patterntile.cpp \
    src/core/patterntransistor.cpp \
    src/core/patterncapacitor.cpp \
    src/core/design.cpp \
    src/printer/designprinter.cpp \
    src/printer/svg.cpp \
    src/spice/spiceparser.cpp\
    src/spice/spiceobject.cpp \
    src/spice/subckt.cpp \
    src/spice/subcktinstance.cpp \
    src/core/instance.cpp \
    src/core/layoutcell.cpp \
    src/gui/window.cpp \
    src/gui/renderarea.cpp \
    src/core/consoleoutput.cpp \
    external/libgds_dist/libgds.c \
    src/printer/gds.cpp

FORMS += \
    src/gui/window.ui

