#!/bin/sh
#
# Startup script to start/stop the server.
#
BIN_69=./sixty-nine
server_BIN=./overworld-server.py

if [ $# -ne 2 ]; then echo "Usage: $0 {start|stop|restart|status} hostname" && exit 1 ; fi

HOSTNAME=$2
CONNECT_BIN="./switchbox-connect $2 5151"


case "$1" in
  start)
        if [ ! -e .serverpid_$hostname ]; then 
            
            echo "$BIN_69 $server_BIN $CONNECT_BIN"
            $BIN_69 $server_BIN "$CONNECT_BIN"
            echo $! > .serverpid_$HOSTNAME
        else 
            echo "Can not start server. Already running. (Or stale serverpid file)"
            exit 1
        fi 
        ;;
  stop)
        if [ -e .serverpid_$HOSTNAME ]; then
            kill `cat .serverpid_$HOSTNAME`
            rm .serverpid_$HOSTNAME
        else 
            echo "Can not stop server. Not running. (Or serverpid file has gone missing.)"
            exit 1
        fi
        ;;
  status)
        if [ -e .serverpid_$HOSTNAME ]; then
            echo "server is running" 
        else
            echo "server is not running"
        fi
        ;;
  restart)
        $0 stop
        $0 start
        ;;
  *)
        echo "Usage: $0 {start|stop|restart|status} HOSTNAME"
        exit 1
esac

exit 0
