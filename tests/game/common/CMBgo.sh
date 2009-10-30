#!/bin/bash 

echo "Starting switchbox" 
~/Fistacuffs/trunk/src/switchbox/switchbox &
#ssh -f ccraig7@hydra3.eecs.utk.edu '~/Fistacuffs/trunk/src/switchbox/switchbox & > whatSheSaid.txt; varpid=`echo $!`'

switchpid=$!
echo `hostname` > server.txt
scp server.txt ccraig7@hydra1.eecs.utk.edu:.
echo "pid = $switchpid"
echo "hostname = `hostname`"
echo "Running CMB on clients"

#ssh -f ccraig7@cetus4.eecs.utk.edu '~/Fistacuffs/trunk/tests/game/common/CMBclientTest < server.txt > whatIsaid4.txt'
#ssh -f ccraig7@cetus5.eecs.utk.edu '~/Fistacuffs/trunk/tests/game/common/CMBclientTest < server.txt > whatIsaid5.txt'

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
