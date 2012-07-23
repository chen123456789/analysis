#!/bin/bash

if [ ! -d ${1} ]; then
  echo "Directory ${1} does not exists"
  exit 1
fi

if [ ! -d ${2} ]; then
  echo "Directory ${2} does not exists"
  exit 1
fi

PATH1=$(cd ${1}; pwd; cd - > /dev/null)
PATH2=$(cd ${2}; pwd; cd - > /dev/null)

echo ""
echo "Missing files in ${PATH2}:"
rsync --prune-empty-dirs -rnvc --ignore-existing ${PATH1}/ ${PATH2}/ | tail -n +2 | head -n -3
echo ""
echo "Missing files in ${PATH1}:"
rsync --prune-empty-dirs -rnvc --ignore-existing ${PATH2}/ ${PATH1}/ | tail -n +2 | head -n -3
echo ""
echo "Differing files:"
rsync --prune-empty-dirs -rnvc --existing ${PATH2}/ ${PATH1}/ | tail -n +2 | head -n -3
echo ""