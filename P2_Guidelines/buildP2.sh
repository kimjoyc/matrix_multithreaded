#!/bin/bash
if [ $argc = 0 ]; then
   echo "usage: ./cmpfile.sh <sourcefilename.cpp>"
else
execname=`basename $1|sed 's|.cpp||'`
CC   -o $execname -march=native -DUSE_LIBSCI -O3 -fopenmp $1 -L /opt/cray/pe/libsci/22.06.1.3/GNU/9.1/x86_64/lib/ -l sci_gnu_mp
fi
