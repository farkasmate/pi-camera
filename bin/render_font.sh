#!/bin/bash

width=${1:-8}

while read line
do
  echo "${line^^}" | grep '^ *0X[0-9A-F]\{1,\},.*$' &> /dev/null
  if [ ${?} != 0 ]; then continue; fi

  echo "ibase=16;obase=2;${line^^}" | sed -e 's/0X//g' -e 's/[, ]*//g' -e 's/\/\/.*$//g' | bc | sed -e 's/0/ /g' -e 's/1/#/g' | xargs -d '\n' -I {} printf "%${width}s\n" "{}"
done < /dev/stdin
