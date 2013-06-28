#!/bin/zsh

# Pack example programs for WWW

set -e

if [[ ! -d examples ]]
then
  print "examples directory not found!"
  exit 1
fi

print cleaning...
examples/clean.sh
print

find examples -name "*.sh"     -o \
              -name "*.[chfi]" -o \
              -name "*.tcl"    -o \
              -name "*.swift" | \
     xargs tar cfz examples.tar.gz
