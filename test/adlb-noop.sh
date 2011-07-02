#!/bin/bash

set -x

THIS=$0
SCRIPT=${THIS%.sh}.tcl
OUTPUT=${THIS%.sh}.out

mpiexec -n 4 bin/turbine ${SCRIPT} >& ${OUTPUT}
[[ ${?} == 0 ]] || exit 1

exit 0
