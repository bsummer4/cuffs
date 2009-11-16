#!/bin/bash 

echo "Starting switchbox" 
~/Fistacuffs/trunk/src/switchbox/switchbox 5151&

switchpid=$!
me=`whoami`
ppath=`pwd`
echo `hostname` > server.txt
echo "pid = $switchpid"
echo "hostname = `hostname`"
echo "me = $me"
#scp server.txt $me@star.eecs.utk.edu:. #Throw it on the eecs domain for reading.Bad, I know.

echo "Running CMBsingle Test"
./CMBsingleTest > singleTest.txt

echo "Running CMB on clients"
ssh -f $me@cetus6.eecs.utk.edu "cd $ppath; ./CMBclientTest < server.txt > client3.txt"
ssh -f $me@cetus7.eecs.utk.edu "cd $ppath; ./CMBclientTest < server.txt > client2.txt"
sleep 10


echo "Killing Switchbox"
echo "[kill $switchpid]"
kill $switchpid 
