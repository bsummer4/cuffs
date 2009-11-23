#!/bin/bash 

echo "Starting switchbox" 
../../../../src/switchbox/switchbox 5151&

switchpid=$!

echo "pid = $switchpid"

echo "[rm -f 1.test_simple 1.test_cmb 1.test_newcmb]"
rm -f 1.test_simple 1.test_cmb 1.test_newcmb 

echo "[./SimpleSynchronizer_test < 1.in > 1.test_simple]"
./SimpleSynchronizer_test < 1.in > 1.test_simple
GOT=`sort 1.test_simple`
SHOULDGET=`sort 1.simple`
if [ "$GOT" != "$SHOULDGET" ] ; then 
    kill $switchpid 
    exit 1
fi

echo "[./CMBSynchronizer_Test < 1.in > 1.test_cmb]"
./CMBSynchronizer_Test < 1.in > 1.test_cmb 
diff -q 1.test_cmb 1.cmb
if [ "$?" != "0" ] ; then 
    kill $switchpid 
    exit 1
fi

echo "[./CMBSynchronizer_Test < 1.in_newcmb > 1.test_newcmb]"
./CMBSynchronizer_Test < 1.in_newcmb > 1.test_newcmb 
diff -q 1.test_newcmb 1.newcmb
if [ "$?" != "0" ] ; then 
    kill $switchpid 
    exit 1
fi

echo "Killing Switchbox"
echo "[kill $switchpid]"
kill $switchpid 
