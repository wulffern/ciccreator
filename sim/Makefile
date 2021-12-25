######################################################################
##        Copyright (c) 2017 Carsten Wulff Software, Norway 
## ###################################################################
## Created       : wulff at 2017-4-17
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

spice=aimspice -o csv 

all: cmp bssw sar

sar:
	${spice} tb_sar9b.cir

bssw:
	${spice} tb_sarbssw.cir

cmp:
	${spice} tb_sarcmp.cir

plot_sar:
	./plot.py tb_sar9b.csv "v(sar_in),v(sar_ip),v(sarn),v(sarp):v(ck_sample),v(done)"
