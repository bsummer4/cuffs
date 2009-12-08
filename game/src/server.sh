#!/bin/sh
#
# Startup script to start/stop the server.
#
server_BIN=./overworld-server.py

if [ $# -ne 2 ]; then echo "Usage: $0 {start|stop|restart|status}" && exit 1 ; fi

GAMENUM=$2


case "$1" in
  start)
        if [ ! -e .serverpid_$GAMENUM ]; then 
            $server_BIN $2 2>&1 > /dev/null & 
            echo $! > .serverpid_$GAMENUM
        else 
            echo "Can not start server. Already running. (Or stale serverpid file)"
            exit 1
        fi 
        ;;
  stop)
        if [ -e .serverpid_$GAMENUM ]; then
            kill `cat .serverpid_$GAMENUM`
            rm .serverpid_$GAMENUM
        else 
            echo "Can not stop server. Not running. (Or serverpid file has gone missing.)"
            exit 1
        fi
        ;;
  status)
        if [ -e .serverpid_$GAMENUM ]; then
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
        echo "Usage: $0 {start|stop|restart|status} GAMENUM"
        exit 1
esac

exit 0
