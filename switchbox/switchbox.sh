#!/bin/sh
#
# Startup script to start/stop the switchbox.
#

usage="Usage: $0 {start|stop|restart|status} port"
[ $# -ne 2 ] && echo $usage && exit 1
port=$2

case "$1" in
  start)
        if [ ! -e .switchboxpid_$port ]; then
            switchbox $2 2>&1 > /dev/null &
            echo $! > .switchboxpid_$port
        else
            echo "Can not start switchbox. Already running. (Or stale switchboxpid file)"
            exit 1
        fi
        ;;
  stop)
        if [ -e .switchboxpid_$port ]; then
            kill `cat .switchboxpid_$port`
            rm .switchboxpid_$port
        else
            echo "Can not stop switchbox. Not running. (Or switchboxpid file has gone missing.)"
            exit 1
        fi
        ;;
  status)
        if [ -e .switchboxpid_$port ]; then
            echo "Switchbox is running"
        else
            echo "Switchbox is not running"
        fi
        ;;
  restart)
        $0 stop $port
        $0 start $port
        ;;
  *)
        echo $usage
        exit 1
esac

exit 0
