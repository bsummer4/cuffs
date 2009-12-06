#!/bin/sh
#
# Startup script to start/stop the switchbox.
#
SWITCHBOX_BIN=../../switchbox/src/switchbox

if [ $# -ne 2 ]; then echo "Usage: $0 {start|stop|restart|status} PORT" && exit 1 ; fi

PORT=$2


case "$1" in
  start)
        if [ ! -e .switchboxpid_$PORT ]; then 
            $SWITCHBOX_BIN $2 2>&1 > /dev/null & 
            echo $! > .switchboxpid_$PORT
        else 
            echo "Can not start switchbox. Already running. (Or stale switchboxpid file)"
            exit 1
        fi 
        ;;
  stop)
        if [ -e .switchboxpid_$PORT ]; then
            kill `cat .switchboxpid_$PORT`
            rm .switchboxpid_$PORT
        else 
            echo "Can not stop switchbox. Not running. (Or switchboxpid file has gone missing.)"
            exit 1
        fi
        ;;
  status)
        if [ -e .switchboxpid_$PORT ]; then
            echo "Switchbox is running" 
        else
            echo "Switchbox is not running"
        fi
        ;;
  restart)
        $0 stop
        $0 start
        ;;
  *)
        echo "Usage: $0 {start|stop|restart|status} PORT"
        exit 1
esac

exit 0
