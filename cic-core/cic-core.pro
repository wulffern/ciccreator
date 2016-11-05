######################################################################
##        Copyright (c) 2015 Carsten Wulff Software, Norway 
## ###################################################################
## Created       : wulff at 2015-8-20
## ###################################################################
##   This program is free software: you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, either version 3 of the License, or
##   (at your option) any later version.
## 
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
## 
##   You should have received a copy of the GNU General Public License
##   along with this program.  If not, see <http://www.gnu.org/licenses/>.
######################################################################

  QT += core

  CONFIG += qt console debug
  CONFIG += staticlib
  CONFIG += c++11

mac {
    mac::QMAKE_MAC_SDK = macosx10.12
}
  TEMPLATE = lib

  osx:DESTDIR = ../lib/darwin
  linux:DESTDIR = ../lib/linux

  #- Add coverage information
  QMAKE_CFLAGS += $$(CFLAGS) 
  QMAKE_LFLAGS += $$(LDFLAGS)

  TARGET = cic

  OBJECTS_DIR=build
  MOC_DIR=build

  DEPENDPATH +=.

  INCLUDEPATH += src/ external/libgds_dist/

  # Input
  HEADERS +=  src/core/rules.h  \
  src/core/rect.h \
  src/printer/minecraft.h \
  src/printer/tikz.h \
        src/printer/eps.h \
    src/core/graph.h \
  src/core/cell.h \
  src/core/instance.h \
  src/core/layoutcell.h \
  src/core/patterntile.h \
  src/core/patterntransistor.h \
  src/spice/spiceobject.h \
  src/spice/mosfet.h \
  src/spice/capacitor.h \
    src/spice/resistor.h \
    src/core/patterncapacitor.h \
    src/core/patternresistor.h \
  src/core/design.h \
  src/printer/gds.h \
  src/printer/designprinter.h \
  src/printer/svg.h \
  src/spice/spiceparser.h\
  src/spice/subckt.h \
  src/spice/subcktinstance.h \
  src/core/consoleoutput.h \
  external/libgds_dist/libgds.h \
  src/core/layer.h \
  src/printer/spice.h \
  src/core/port.h \
  src/core/route.h \
  src/core/routering.h \
  src/core/cut.h \
  src/core/instancecut.h \
    src/core/instanceport.h \
    src/core/layoutrotatecell.h \
    src/core/text.h \
    src/core/point.h \
    src/cells/cells.h \
    src/cells/sar.h \
    src/cells/cdac.h \
    src/cells/capcell.h


  SOURCES +=         src/printer/gds.cpp \
  src/core/rules.cpp \
  src/core/rect.cpp \
  src/core/cell.cpp   \
  src/printer/minecraft.cpp \
  src/printer/tikz.cpp \
    src/printer/eps.cpp \
  src/core/patterntile.cpp \
  src/core/patterntransistor.cpp \
  src/core/patterncapacitor.cpp \
      src/core/patternresistor.cpp \
  src/core/design.cpp \
  src/printer/designprinter.cpp \
  src/printer/svg.cpp \
  src/spice/spiceparser.cpp \
  src/spice/spiceobject.cpp \
  src/spice/subckt.cpp \
  src/spice/subcktinstance.cpp \
  src/core/instance.cpp \
  src/core/layoutcell.cpp \
  src/core/layer.cpp \
  src/core/consoleoutput.cpp \
  external/libgds_dist/libgds.c \
  src/printer/spice.cpp \
  src/core/port.cpp\
  src/spice/mosfet.cpp\
  src/spice/capacitor.cpp \
  src/spice/resistor.cpp \
  src/core/route.cpp \
  src/core/routering.cpp \
  src/core/cut.cpp \
  src/core/instanceport.cpp \
  src/core/layoutrotatecell.cpp \
  src/core/text.cpp \
  src/cells/sar.cpp\
  src/cells/cdac.cpp \
  src/cells/capcell.cpp
