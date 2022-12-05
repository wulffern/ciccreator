#!/bin/bash
######################################################################
##        Copyright (c) 2015 Carsten Wulff Software, Norway
## ###################################################################
## Created       : wulff at 2015-7-23
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
platform=`uname -a | awk '{ print tolower($1)}'`

appname=`basename $0 | sed s,\.sh$,,`

dirname=`dirname $0`
tmp="${dirname#?}"

if [ "${dirname%$tmp}" != "/" ]; then
dirname=$PWD/$dirname
fi

#- Setup LD library path on linux
if [ $platform == "linux" ] ; then

LD_LIBRARY_PATH=$dirname
export LD_LIBRARY_PATH

execfile=$dirname/${platform}/${appname}

if [ ! -f "$execfile" ]; then
echo "INFO: Trying pre-compiled "
execfile=${dirname}/../release/${appname}.${platform}-latest
fi

echo $execfile "$@"

$execfile "$@"

else

    if [ $platform == "darwin" ]; then


    execfile=$dirname/${platform}/${appname}.app/Contents/MacOS/${appname}
    if [ ! -f "$execfile" ]; then
    execfile=$dirname/${platform}/${appname}
    fi

    if [ ! -f "$execfile" ]; then
    echo "INFO: Trying pre-compiled "
    execfile=${dirname}/../release/${appname}.${platform}-latest
    fi

    echo $execfile "$@"
    $execfile "$@"



    else
    echo "Looks like your on windows. Windows no longer supported, use linux in WSL"
    fi
fi
