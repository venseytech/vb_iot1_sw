#!/bin/sh

##############################################################################
# Example Build script
##############################################################################

SCRIPT=$(readlink -f $0)
SCRIPTDIR=$(dirname $SCRIPT)/../
echo SCRIPT = ${SCRIPT} 
echo SCRIPTDIR = ${SCRIPTDIR}
echo $(date) 
echo VeNSeY: Example Build Operation for Training Purposes 
 
make O=$SCRIPTDIR/output BR2_EXTERNAL=$SCRIPTDIR -C $SCRIPTDIR/buildroot $*

