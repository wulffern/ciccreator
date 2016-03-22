#-------------------------------------------------
#
# Project created by QtCreator 2016-03-12T18:50:58
#
#-------------------------------------------------

QT += core widgets
CONFIG += qt console debug

  mac {
    CONFIG -= app_bundle
  }

  TEMPLATE = app

  osx:DESTDIR = ../bin/darwin
  linux:DESTDIR = ../bin/linux

  #- Add coverage information
  QMAKE_CFLAGS += $$(CFLAGS) --coverage
  QMAKE_LFLAGS += $$(LDFLAGS) --coverage

  TARGET = cic-gui

  OBJECTS_DIR=build
  MOC_DIR=build

  DEPENDPATH +=.

  INCLUDEPATH += src/ ../cic-core/src ../cic-core/external/libgds_dist


SOURCES += src/main.cpp\
src/renderarea.cpp\
src/window.cpp


HEADERS  += src/renderarea.h \
src/window.h 

#FORMS    += cicgui.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/darwin/release/ -lcic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/darwin/debug/ -lcic
else:linux: LIBS += -L$$PWD/../lib/linux/ -lcic
else:mac: LIBS += -L$$PWD/../lib/darwin/ -lcic

mac:PRE_TARGETDEPS += ../lib/darwin/libcic$${LIBSUFFIX}.a
mac:INCLUDEPATH += $$PWD/../lib/darwin
                   mac:DEPENDPATH += $$PWD/../lib/darwin

                   
                   linux:PRE_TARGETDEPS += ../lib/linux/libcic$${LIBSUFFIX}.a
linux:INCLUDEPATH += $$PWD/../lib/linux
                   linux:DEPENDPATH += $$PWD/../lib/linux

