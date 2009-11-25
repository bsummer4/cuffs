#!/usr/bin/python

#module
'''
 # Usage

 ./ref numberofplayers

 The first group of names belong to team1 and the second group
 belong to team 2.

 Ref will:
   * Waits for all clients to join (They will need to '/identify' themselves)
   * Place all the player objects ('/new player-[player-id] player x y')
   * Starts Game '/start'
   * This all happens before the cmb is started. Clients start their CMB's
     when they see the /start message.
   * Montors the game and calculates statistics
'''

import sys, os
sys.path.append("../../../switchbox") # where 'client' is
sys.path.append("../../../../utils") # where 'switchbox-cat' is
import client
from client import Connection
import switchbox-cat import  

team1list = team2list = None # globals
messageQ = []

def addplayer(player, id):
  print "Player ", player, " has joined\n"
  tmp = '/new player-%s player %d %d'.format(player, 50, 50)
  messageQ.append(tmp)

def go():
  c.broadcast('/map ../../../../gamefiles/battle/maps/map1.pgm') 
  c.broadcast('/start')
  for tmp in messageQ:
    c.broadcast(tmp)

def waiting4players(num):
  pcount = 0;
  while True:
    message = c.receive()
    line = client.copy_message_string(message)
    if '/identify' in line:
      print pcount, len(team1list), len(team2list)
      command, player = line.split()
      addplayer(player, pcount)
      pcount = pcount + 1
    if pcount == num:
      return

if __name__ == '__main__':
  if len(sys.argv) != 5:
    print "usage: ./ref hostname port numberofplayers"
    sys.exit(1)

  command, hostname, sbport, numberofplayers = sys.argv

  c = Connection(hostname, int(sbport))

  waiting4players(numberofplayers)
  go()
