#!/bin/sh

# TODO sanitize user input so that > and < (for example) can be
# included.  Markdown isn't much use without them.

./process-mail.py | ./post-unfuddle-message.sh dashinggents 1 $1 $2 3


