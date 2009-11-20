#!/usr/bin/python

#module
'''
 How to use:
 ./ref name1,name2,name3,name4 name5,name6,name7,name8
 
 The first group of names belong to team1 and the second group belong to team 2.
 Ref will:
   * Waits for all clients to join (They will need to '/identify' themselves)
   * Place all the player objects ('/new player-[player-id] player x y')
   * Starts Game '/start'
   * This all happens before the cmb is started. Clients start their CMB's 
     when they see the /start message.

Assumptions:
commands are
/identify playername

'''
import sys, os
sys.path.append("../src/switchbox")
import client
from client import Connection
from threading import Thread


if len(sys.argv) != 3:
  print "./ref name1,name2... name5,name6... sbport, target"
  sys.exit(1)

tmp, team1, team2, sbport, target = sys.argv
team1list = team1.split(',')
team2list = team2.split(',')
c = Connection('localhost', int(sbport)); #assumed this is running on the switchbox;

def addplayer(player, id, target):
  print "Player ", player, " has joined\n"
  tmp = '/new {}-[{}] {} {} {}'.format(pcount,player, '50','50')
  command = c.send(target,tmp)

def go():
  c.send(target, '/start')
  

def waiting4players():
  while True: 
    message = c.receive()
    line = client.copy_message_string(message) 
    pcount = 0;
    if(identify in line):
      command, player = line.split()
      addplayer(player, pcount, target)
      pcount = pcount + 1
    if(pcount == ( len(team1list)+len(team2list)) ):
      return

#main
waiting4players()
go()
