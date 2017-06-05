#!/usr/bin/env bash
set -e

THISDIR=$( dirname $0 )
source ${THISDIR}/swift-t-settings.sh

if (( ! RUN_MAKE )); then
  exit
fi

if (( MAKE_CLEAN ))
then
  if [ -f Makefile ]
  then
    make clean
  fi
fi

make -j ${MAKE_PARALLELISM}
