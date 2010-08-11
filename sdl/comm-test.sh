#!/bin/sh

while read x
do	case $x in
		up) echo 'shift .c3 0 -10';;
		down) echo 'shift .c3 0 10';;
		left) echo 'shift .c3 -10 0';;
		right) echo 'shift .c3 10 0';;
		shoot*) echo 'explode 40 {*}[.cursor pos]'
	esac
	echo $x > /dev/stderr
done
