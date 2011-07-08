#!/bin/bash

set -x

THIS=$0
SCRIPT=${THIS%.sh}.tcl
OUTPUT=${THIS%.sh}.out

bin/turbine ${SCRIPT} >& ${OUTPUT}
[[ ${?} == 0 ]] || exit 1

LINES=$( ls test/data/[ABCD].txt | wc -l )
(( ${LINES} == 4 )) || exit 1

rm [ABCD].txt || exit 1

exit 0
