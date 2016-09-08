######################################################################
##        Copyright (c) 2016 Carsten Wulff Software, Norway 
## ###################################################################
## Created       : wulff at 2016-8-7
## ###################################################################
## The MIT License (MIT)
## 
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
## copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
## 
## The above copyright notice and this permission notice shall be included in all
## copies or substantial portions of the Software.
## 
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
## SOFTWARE.
## 
######################################################################


  QT += core widgets

  CONFIG += qt console debug
  CONFIG += c++11
  mac {
    CONFIG -= app_bundle
  }

  TEMPLATE = app

  osx:DESTDIR = ../bin/darwin
  linux:DESTDIR = ../bin/linux

  #- Add coverage information
  QMAKE_CFLAGS += $$(CFLAGS) --coverage
  QMAKE_LFLAGS += $$(LDFLAGS) --coverage

  TARGET = cic-color

  OBJECTS_DIR=build
  MOC_DIR=build

  DEPENDPATH +=.

  INCLUDEPATH += src

  # Input

  SOURCES += src/renderarea.cpp \
   src/window.cpp \
   src/main.cpp 

  
  HEADERS  +=   src/renderarea.h src/window.h 
