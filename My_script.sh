#!/bin/sh

NAMES=$(find -type f)
for CUR_NAME in $NAMES; do
  tmp=$(cat $CUR_NAME | grep "pflags")  
  if [ "$tmp" ]; then
    echo "$CUR_NAME"
    echo $tmp
    echo 
  fi
done
