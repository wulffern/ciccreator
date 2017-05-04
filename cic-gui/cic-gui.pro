#-------------------------------------------------
#
# Project created by QtCreator 2016-03-12T18:50:58
#
#-------------------------------------------------

QT += core widgets
CONFIG += qt console debug
TEMPLATE = app
mac {
mac::QMAKE_MAC_SDK = macosx10.12
ICON = ALGIC003_STDLIB.icns
}

  TARGET = cic-gui

  SOURCES +=  src/widget.cpp\
  src/window.cpp
  

HEADERS  += src/widget.h \
src/window.h 

include(../etc/console_config.pro)
