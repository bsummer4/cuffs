#!/usr/bin/python

#module
'''
 # Usage

 ./ref name1,name2,name3,name4 name5,name6,name7,name8 sbport

 The first group of names belong to team1 and the second group
 belong to team 2.

 Ref will:
   * Waits for all clients to join (They will need to '/identify' themselves)
   * Place all the player objects ('/new player-[player-id] player x y')
   * Starts Game '/start'
   * This all happens before the cmb is started. Clients start their CMB's
     when they see the /start message.

 # Assumptions
 Commands are:
   * /identify playername
'''

import sys, os
sys.path.append("../../../switchbox") # where 'client' is
import client
from client import Connection

team1list = team2list = None # globals

def addplayer(player, id):
  print "Player ", player, " has joined\n"
  tmp = '/new player-%s player %d %d'%(player, 50, 50)
  command = c.broadcast(tmp)

def go():
  # TODO broadcast the map
  c.broadcast('/start')
  # TODO Maybe it would be better to send the '/new' messages after
  # '/start'.  You could just add them all to a global list of
  # messages then send them all here

def waiting4players():
  pcount = 0;
  while True:
    message = c.receive()
    line = client.copy_message_string(message)
    if '/identify' in line:
      print pcount, len(team1list), len(team2list)
      command, player = line.split()
      addplayer(player, pcount)
      pcount = pcount + 1
    if pcount == len(team1list) + len(team2list):
      return

if __name__ == '__main__':
  if len(sys.argv) != 4:
    # TODO "./ref hostname port team1list team2list" would be better
    print "usage: ./ref name1,name2... name5,name6... sbport"
    sys.exit(1)

  command, team1, team2, sbport = sys.argv
  team1list = team1.split(',')
  team2list = team2.split(',')
  # TODO localhost should not be assumed
  c = Connection('localhost', int(sbport))

  waiting4players()
  go()
