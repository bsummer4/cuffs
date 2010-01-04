#!/bin/sh

# Startup script to start/stop the server.

usage="Usage: $0 {start|stop|restart|status}"
[ $# -ne 1 ] && echo $usage && exit 1
hostname=localhost
port=38235

could_not_then_die () {
  echo "Can not $1 server.  $2."
  exit 1
}

case "$1" in
  start)
        if [ ! -e .serverpid_$hostname ];
        then
            sixty-nine overworld-server.py \
                "switchbox-connect $hostname $port" &
            echo $! > .serverpid_$hostname
        else could_not_then_die start "Already running or stale pid file"
        fi
        ;;
  stop)
        if [ -e .serverpid_$hostname ]
        then
            kill `cat .serverpid_$hostname`
            rm .serverpid_$hostname
        else could_not_then_die stop "Not running or deleted pid file"
        fi
        ;;
  status)
        if [ -e .serverpid_$hostname ];
        then echo "server is running"
        else echo "server is not running"
        fi
        ;;
  restart)
        $0 stop
        $0 start
        ;;
  *)
        echo $usage
        exit 1
esac
