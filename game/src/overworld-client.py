#!/usr/bin/python

import wx, sys, threading, os, time

underworld_port = 5151

def add_new(item, list):
    if item in list: return list
    return list + [item]

# Overworld GUI Frame
class OverworldFrame(wx.Frame):
  def __init__(self, oworld, *args, **kwds):
    kwds["style"] = wx.DEFAULT_FRAME_STYLE
    wx.Frame.__init__(self, *args, **kwds)
    self.world = oworld
    self.PlayerList = wx.ListBox (
        self, -1, choices=[],
        style=wx.LB_MULTIPLE|wx.LB_EXTENDED|wx.LB_NEEDED_SB,
        size=(150,400))
    self.Play = wx.Button(self, -1, "Play", style=wx.BU_EXACTFIT)
    self.Logout = wx.Button(self, -1, "Logout")
    self.Send = wx.Button(self, -1, "Send")
    self.ChatInput = wx.TextCtrl(self, 1)
    self.ChatBox = wx.TextCtrl(self, 1, style=wx.TE_MULTILINE|wx.TE_READONLY)

    self.__set_properties()
    self.__do_layout()
    self.Bind(wx.EVT_BUTTON, self.OnPlay, id=self.Play.GetId())
    self.Bind(wx.EVT_BUTTON, self.OnLogout, id=self.Logout.GetId())
    self.Bind(wx.EVT_BUTTON, self.OnSend, id=self.Send.GetId())
    self.Bind(wx.EVT_CLOSE, self.OnLogout)

  def __set_properties(self):
    self.SetTitle("Fistacuffs Lobby")

  def __do_layout(self):
    OverFrameSizer = wx.FlexGridSizer(4, 1, 5, 5)
    OverFrameSizer.Add(self.PlayerList, 0,
                       wx.EXPAND|wx.ALIGN_CENTER_HORIZONTAL, 0)
    ButtonBox = wx.BoxSizer(wx.HORIZONTAL)
    ButtonBox.Add((0, 0), 1, wx.EXPAND, 0)
    ButtonBox.Add(self.Play, 1, wx.EXPAND, 0)
    ButtonBox.Add(self.Logout, 1, wx.EXPAND, 0)
    ButtonBox.Add(self.Send, 1, wx.EXPAND, 0)
    ButtonBox.Add((0, 0), 1, wx.EXPAND, 0)
    OverFrameSizer.Add(self.ChatBox, 1,
                       wx.EXPAND|wx.ALIGN_CENTER_HORIZONTAL, 0)
    OverFrameSizer.Add(self.ChatInput, 1,
                       wx.EXPAND|wx.ALIGN_CENTER_HORIZONTAL, 0)
    OverFrameSizer.Add(ButtonBox, 1, wx.EXPAND|wx.ALIGN_CENTER_HORIZONTAL, 0)
    self.SetSizer(OverFrameSizer)
    OverFrameSizer.Fit(self)
    OverFrameSizer.AddGrowableRow(0)
    OverFrameSizer.AddGrowableCol(0)
    self.Layout()

  def SelectedPlayers(self):
    return map(self.PlayerList.GetString, self.PlayerList.GetSelections())

  def OnSend(self, event):
      print self.ChatInput.GetValue()
      sys.stdout.flush()

  def OnPlay(self, event):
    print "/play", " ".join(add_new(self.world.username,
                                    self.SelectedPlayers()))
    sys.stdout.flush()

  def OnLogout(self, event):
    print "/logout " + self.world.username
    sys.exit(0) # TODO Does wx have a 'correct' way of closing out?

class Overworld(wx.App):
  def __init__(self, username, hostname):
    self.username = username
    self.hostname = hostname
    wx.App.__init__(self)

  def OnInit(self):
    self.OverFrame = OverworldFrame(self, None, -1, "")
    self.OverFrame.Show(True)
    self.SetTopWindow(self.OverFrame)
    print "/login", self.username
    print "/list" # This gets the currently logged in players.  After
                  # this we just keep track of /login, /logout
                  # messages ourself
    sys.stdout.flush()
    return True

def listen(OverFrame):
  while True:
    sys.stderr.write('-- ');
    sys.stderr.flush()
    line_text = sys.stdin.readline()
    sys.stderr.write("got line: " + line_text)
    sys.stderr.flush()
    if not line_text: break # EOF
    OverFrame.OverFrame.ChatBox.AppendText(line_text)
    line = line_text.split()
    if len(line) == 0 or len(line) == 1: continue # empty input
    sender, command, args = line[0], line[1], line[2:]
    if command == '/login':
      for arg in args:
        app.OverFrame.PlayerList.Append(arg)
    elif command == '/logout':
      for arg in args:
        app.OverFrame.PlayerList.Delete (
            app.OverFrame.PlayerList.FindString(arg))
    elif command == '/players':
      app.OverFrame.PlayerList.Set(args)
    elif command == '/play':
      print '/logout ' + app.username
      if app.username in args:
        time.sleep(0.5)
        os.execlp("./sixty-nine", "./sixty-nine", './game %s'%(app.username),
                  './switchbox-connect %s %d'%(app.hostname, underworld_port))

if __name__ == "__main__":
  program, username, hostname = sys.argv
  app = Overworld(username, hostname)
  t = threading.Thread(target=listen, name="MainThread", args=(app,))
  t.start()
  app.MainLoop()
