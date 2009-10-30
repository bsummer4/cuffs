#!/bin/bash 

echo "Starting switchbox" 
../../../src/switchbox/switchbox &
#ssh -f ccraig7@hydra3.eecs.utk.edu '~/Fistacuffs/trunk/src/switchbox/switchbox & > whatSheSaid.txt; varpid=`echo $!`'

switchpid=$!
echo "pid = $switchpid"
echo "Running CMB on clients"

#it crashes here
ssh -f ccraig7@cetus4.eecs.utk.edu '~/Fistacuffs/trunk/test/game/common/CMBclientTest < `hostname` > whatIsaid4.txt'
ssh -f ccraig7@cetus5.eecs.utk.edu '~/Fistacuffs/trunk/test/game/common/CMBclientTest < `hostname` > whatIsaid5.txt'

echo "Killing Switchbox"
echo "[kill $switchpid]"
kill $switchpid 
#echo "[./SimpleSynchronizer_test < 1.in > 1.test_simple]"
#./SimpleSynchronizer_test < 1.in > 1.test_simple
#GOT=`sort 1.test_simple`
#SHOULDGET=`sort 1.simple`
#if [ "$GOT" != "$SHOULDGET" ] ; then 
#    kill $switchpid 
#    exit 1
#fi
#
#echo "[./sync_tests < 1.in > 1.test_cmb]"
#./sync_tests < 1.in > 1.test_cmb 
#diff -q 1.test_cmb 1.cmb
#if [ "$?" != "0" ] ; then 
#    kill $switchpid 
#    exit 1
#fi
#
#echo "Killing Switchbox"
#echo "[kill $switchpid]"
#kill $switchpid 
