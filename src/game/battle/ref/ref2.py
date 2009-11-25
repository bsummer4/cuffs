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
   * Montors the game and calculates statistics
'''

import sys, os
sys.path.append("../../../switchbox") # where 'client' is
sys.path.append("../../../../utils") # where 'switchbox-cat' is
import client
from client import Connection
import switchbox-cat import  
import random

team1list = team2list = None # globals
messageQ = []
coords = []

def getPosition():
  file = open("../../../../gamefiles/battle/maps/map1_start.txt")
  for line in file:
    tmp = line.split(',')
    coords.append([tmp[0],tmp[1]])
  file.close()
    
def addplayer(player, id):
  print "Player ", player, " has joined\n"
  num = random.randrange(0,len(coords))
  tmp = '/new player-%s player %d %d'.format(player, coords[num][0], coords[num][1])
  messageQ.append(tmp)

def go():
  c.broadcast('/map ../../../../gamefiles/battle/maps/map1.pgm') 
  c.broadcast('/start')
  for tmp in messageQ:
    c.broadcast(tmp)

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

def tracking():
  shotcount = 0
  while True:
    message = c.receive()
    line = client.copy_message_string(message)
    if '/shoot' in line:
      shotcount = shotcount + 1

if __name__ == '__main__':
  if len(sys.argv) != 5:
    print "usage: ./ref hostname port name1,name2... name4,name5..."
    sys.exit(1)

  command, hostname, sbport, team1, team2 = sys.argv
  team1list = team1.split(',')
  team2list = team2.split(',')

  c = Connection(hostname, int(sbport))

  waiting4players()
  go()
  tracking()
