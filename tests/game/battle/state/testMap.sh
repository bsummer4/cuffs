#!/bin/bash 
echo "================================================"
echo "Testing if the explosions manipulated the map correctly"
if [ "`diff -q test.pgm test-map-good.pgm`" == "" ]
then
  echo " Pass"
  echo "================================================"
  exit 0
else
  echo " Fail"
  echo "================================================"
  exit 1
fi
