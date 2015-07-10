######################################################################
# Automatically generated by qmake (2.01a) Tue Jul 16 19:48:51 2013
######################################################################

QT += core
QT -= gui


CONFIG += qt console debug
CONFIG -= app_bundle

TEMPLATE = app

TARGET = cic
DESTDIR = bin
OBJECTS_DIR=build
MOC_DIR=build
DEPENDPATH += .

INCLUDEPATH += include/


# Input

HEADERS +=  include/core/rules.h  \
include/core/rect.h \
include/core/cell.h \
include/core/design.h \ 
include/core/patterntile.h \
include/core/patterntransistor.h \
include/core/patterncapacitor.h \
include/printer/designprinter.h \
include/printer/svg.h \
include/spice/spiceparser.h\
include/spice/spiceobject.h \
include/spice/subckt.h \
include/spice/subcktinstance.h \
    include/core/instances.h


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
    src/core/instance.cpp

