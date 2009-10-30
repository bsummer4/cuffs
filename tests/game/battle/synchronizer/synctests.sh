#!/bin/bash 

echo "Starting switchbox" 
../../../../src/switchbox/switchbox &

switchpid=$!

echo "pid = $switchpid"

echo "[rm -f 1.test_simple 1.test_cmb]"
rm -f 1.test_simple 1.test_cmb

echo "[./SimpleSynchronizer_test < 1.in > 1.test_simple]"
./SimpleSynchronizer_test < 1.in > 1.test_simple
GOT=`sort 1.test_simple`
SHOULDGET=`sort 1.simple`
if [ "$GOT" != "$SHOULDGET" ] ; then 
    kill $switchpid 
    exit 1
fi

echo "[./sync_tests < 1.in > 1.test_cmb]"
./sync_tests < 1.in > 1.test_cmb 
diff -q 1.test_cmb 1.cmb
if [ "$?" != "0" ] ; then 
    kill $switchpid 
    exit 1
fi

echo "Killing Switchbox"
echo "[kill $switchpid]"
kill $switchpid 
