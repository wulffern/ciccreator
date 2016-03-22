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

  unix:!mac{
          QMAKE_LIBDIR += ~/qt5_4/lib  ~/qt5_4/plugins/platforms
  }


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

  TARGET = cic

  OBJECTS_DIR=build
  MOC_DIR=build

  DEPENDPATH +=.

  INCLUDEPATH += src/ ../cic-core/external/libgds_dist/ ../cic-core/src

  # Input

  SOURCES +=         src/main.cpp


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

