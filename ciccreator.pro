######################################################################
##        Copyright (c) 2016 Carsten Wulff Software, Norway 
## ###################################################################
## Created       : wulff at 2016-3-13
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

QMAKE_CFLAGS += $$(CFLAGS)  -g
CONFIG += c++11
CONFIG += debug
CONFIG += static


TEMPLATE=subdirs
  CONFIG += ordered
  SUBDIRS = cic-core \
  cic \
  cic-gui \
  cic2eps \
  cic2tikz \
  cic2png \
  cic2scriptcraft \
  cic-color

  cic.depends = cic-core
  cic-gui.depends = cic-core
  cic2eps.depends = cic-core
  cic2tikz.depends = cic-core
  cic2png.depends = cic-core
  cic2scriptcraft.depends = cic-core

