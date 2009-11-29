#!/bin/sh
#
# Startup script to start/stop the switchbox.
#
SWITCHBOX_BIN=../../src/switchbox/switchbox


case "$1" in
  start)
        if [ ! -e switchboxpid ]; then 
            $SWITCHBOX_BIN 3214 2>&1 > /dev/null & 
            echo $! > switchboxpid
        else 
            echo "Can not start switchbox. Already running. (Or stale switchboxpid file)"
            exit 1
        fi 
        ;;
  stop)
        if [ -e switchboxpid ]; then
            kill `cat switchboxpid`
            rm switchboxpid
        else 
            echo "Can not stop switchbox. Not running. (Or switchboxpid file has gone missing.)"
            exit 1
        fi
        ;;
  status)
        if [ -e switchboxpid ]; then
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
        echo "Usage: $0 {start|stop|restart|status}"
        exit 1
esac

exit 0
