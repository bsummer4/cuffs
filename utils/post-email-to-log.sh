#!/bin/sh

./process-mail.py | ./post-unfuddle-message.sh dashinggents 1 $1 $2 3
