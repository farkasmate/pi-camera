#!/bin/bash

while IFS= read line
do
  echo "${line}" | grep '^[ #]*$' &> /dev/null
  if [ ${?} != 0 ]; then continue; fi

  binaries=$(echo "${line}" | sed -e 's/ /0/g' -e 's/#/1/g'  -e 's/\(.\{8\}\)/\1;/g')
  echo "obase=16;ibase=2;${binaries}" | bc | xargs -I {} echo "0X{}" | xargs -d '\n' -I {} printf "0X%02X, " "{}"
  echo ""
done < /dev/stdin
