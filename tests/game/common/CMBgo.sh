#!/bin/bash 

echo "Starting switchbox" 
~/Fistacuffs/trunk/src/switchbox/switchbox 5151&

switchpid=$!
me=`whoami`
echo `hostname` > server.txt
scp server.txt ccraig7@star.eecs.utk.edu:. #Throw it on the eecs domain for reading.Bad, I know.
echo "pid = $switchpid"
echo "hostname = `hostname`"
echo "me = $me"

echo "Running CMBsingle Test"
./CMBsingleTest < f1.txt 
sleep 5

echo "Running CMB on clients"
ssh -f $me@cetus6.eecs.utk.edu '~/Fistacuffs/trunk/tests/game/common/CMBclientTest < server.txt > whatIsaid4.txt'
ssh -f $me@cetus5.eecs.utk.edu '~/Fistacuffs/trunk/tests/game/common/CMBclientTest < server.txt > whatIsaid5.txt'

echo "Killing Switchbox"
echo "[kill $switchpid]"
kill $switchpid 
