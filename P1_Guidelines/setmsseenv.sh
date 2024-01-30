#!/bin/bash
tgtname="bash"
rootname=`echo $0 | sed s/-//`
if [ "$rootname" != "$tgtname" ]; then
   echo "This file must run under source"
   echo "usage: source setmsseenv.sh"
else
   echo "setting the environment"
   module load cray-libsci/22.11.1.2
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/cray/pe/libsci/22.11.1.2/GNU/9.1/x86_64/lib/
   export OMP_NUM_THREADS=4
fi
~ 
