######################################################################
# Automatically generated by qmake (2.01a) Sat Aug 14 06:28:31 2010
######################################################################
QT +=  core widgets
CONFIG += c++11
CONFIG += qt console debug

TEMPLATE = app 
TARGET = cschematic

OBJECTS_DIR=build
MOC_DIR=build


osx:DESTDIR = ../bin/darwin
linux:DESTDIR = ../bin/linux

DEPENDPATH += . 
INCLUDEPATH += src/

mac {
mac::QMAKE_MAC_SDK = macosx10.12
}


# Input
HEADERS += src/mainwindow.h \
     src/geda/gobject.h \
     src/geda/gcolors.h \
     src/math/gmath.h \
     src/defines/colors.h \
     src/defines/o_types.h \
     src/defines/defines.h \
     src/geda/gEdaFileReader.h \
     src/geda/gEdaFileWriter.h \
     src/geda/gEdaObjectFactory.h \
     src/geda/gEdaSymbolLibrary.h \
     src/items/captureitem.h \
     src/schematic/capturescene.h \
     src/schematic/captureview.h \
     src/commands/scenemodifier.h \
     src/commands/commands.h 

FORMS += src/mainwindow.ui
SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/geda/gobject.h \
           src/geda/gcolors.h \
           src/math/gmath.cpp \
           src/geda/gEdaFileReader.h \
           src/geda/gEdaFileWriter.cpp \
           src/geda/gEdaObjectFactory.h \
           src/geda/gEdaSymbolLibrary.cpp \
           src/items/captureitem.cpp \
           src/schematic/capturescene.cpp \
           src/schematic/captureview.cpp \
           src/commands/scenemodifier.cpp \
           src/commands/commands.cpp           