#!/usr/bin/python

"""
Usage: overServer.py
Maintains a list of players currently on the switchbox
interfaces with the rest of the game using stdin and stdout
"""

import sys, os, random, time

playerlist = []
game_port = 5151

def playerUpdate():
    while True:
        line_text = sys.stdin.readline()
        if not line_text: return # EOF
        if not line_text.strip(): continue
        line = line_text.split()
        if len(line) <= 1: continue
        (sender, command), args = line[0:2], line[2:]
        if command == '/login':
            playerlist.append(args[0])
        elif command == '/logout':
            playerlist.remove(args[0])
        elif command == '/list':
            sys.stdout.write("/players " + " ".join(playerlist) + "\n")
        elif command == '/play':
            os.system("switchbox.sh start %d > /dev/stderr" % game_port)
            time.sleep(0.05) # just in case
            os.system (
                "sixty-nine 'ref.py %s annoations.txt' 'switchbox-connect localhost %d'" \
                    % (' '.join(args), game_port))
            os.system("switchbox.sh stop %d > /dev/stderr" % game_port)
        sys.stdout.flush()

if __name__ == '__main__':
    if len(sys.argv) != 1:
        print __doc__
        exit(1)
    playerUpdate()