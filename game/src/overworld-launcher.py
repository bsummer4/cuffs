#!/usr/bin/python

import wx, sys, threading, os, time
from functools import partial
underworld_port = 5151
overworld_port = 38235

kill_window = lambda: None # Will we set to main to hide any shown windows

def print_(x):
  print x
  sys.stdout.flush()
  return x

def launch(run_server, username, hostname):
  connect = './switchbox-connect %s %s'%(hostname, overworld_port)
  print_(1)
  client = './overworld-client.py %s %s'%(username, hostname)
  print_(1)
  if run_server:
    print_(1)
    if os.system("./switchbox.sh start %d" % overworld_port):
      raise Exception("Unable to start switchbox")
    print_(1)
    if os.system("./server.sh start"):
      os.system("./switchbox.sh stop %d" % overworld_port)
      raise Exception("Unable to start overworld-server")
  kill_window()
  print_(1)
  os.system("./sixty-nine '%s' '%s'" % (connect, client))
  if run_server:
    os.system("./switchbox.sh stop %d" % overworld_port)
    os.system("./server.sh stop")

# Launcher GUI Frame
class LauncherFrame(wx.Frame):
  def __init__(self, launcher, *args, **kwds):
    kwds["style"] = wx.DEFAULT_FRAME_STYLE
    wx.Frame.__init__(self, size=(600,400), *args, **kwds)
    self.userLabel = wx.StaticText(self, -1, 'Username:', name='userlabel')
    self.userBox = wx.TextCtrl(self,-1, name='user')
    self.hostLabel = wx.StaticText(self, -1, 'Hostname:', name='hostlabel')
    self.hostBox = wx.TextCtrl(self,-1, name='host')
    self.hostBox.SetValue('localhost')
    self.servercheck = wx.CheckBox(self, -1, 'Run Server?', name='servercheck')
    self.go = wx.Button(self, -1, "Go")
    self.quit = wx.Button(self, -1, "NoGo")
    self.launcher = launcher

    self.__set_properties()
    self.__do_layout()
    self.Bind(wx.EVT_BUTTON, self.OnGo, id=self.go.GetId())
    self.Bind(wx.EVT_BUTTON, self.OnNoGo, id=self.quit.GetId())
    self.Bind(wx.EVT_TEXT, self.OnChange, id=self.userBox.GetId())
    self.Bind(wx.EVT_TEXT, self.OnChange, id=self.hostBox.GetId())
    self.Bind(wx.EVT_CHECKBOX, self.OnCheck, id=self.servercheck.GetId())

  def __set_properties(self):
    self.SetTitle("Fistacuffs Launcher")

  def __do_layout(self):
    oversizer = wx.FlexGridSizer(2, 1, 5, 5)
    hbox = wx.FlexGridSizer(1,4,5,5)
    vbox = wx.BoxSizer(wx.VERTICAL)
    buttonsizer = wx.BoxSizer(wx.HORIZONTAL)

    vbox.Add(self.hostLabel, flag = wx.ALIGN_CENTER)
    vbox.Add(self.servercheck, flag = wx.ALIGN_CENTER)

    hbox.Add(self.userLabel, 0, wx.ALIGN_CENTER, 0)
    hbox.Add(self.userBox, 1, wx.ALIGN_CENTER, 1)
    hbox.Add(vbox, 0, wx.EXPAND, 2)
    hbox.Add(self.hostBox, 1, 0, 3)

    buttonsizer.Add((0,0), 1, wx.EXPAND, 0)
    buttonsizer.Add(self.go)
    buttonsizer.Add(self.quit)
    buttonsizer.Add((0,0), 1, wx.EXPAND, 0)

    oversizer.Add(hbox, 0, wx.EXPAND|wx.ALIGN_CENTER_HORIZONTAL, 0)
    oversizer.Add(buttonsizer, 1, wx.EXPAND|wx.ALIGN_CENTER_HORIZONTAL, 0)

    self.SetSizer(oversizer)
    oversizer.Fit(self)
    oversizer.AddGrowableRow(0)
    self.Layout()

  def OnGo(self, event):
    launch(self.servercheck.IsChecked(),
           self.launcher.username, self.launcher.hostname)
    exit(0)

  def OnNoGo(self, event):
    sys.exit(0)

  def OnChange(self, event):
    self.launcher.username = self.userBox.GetValue()
    self.launcher.hostname = self.hostBox.GetValue()

  def OnCheck(self, event):
    if self.servercheck.IsChecked():
      self.hostBox.SetValue("localhost")
      self.hostBox.SetEditable(False)
    else:
      self.hostBox.SetEditable(True)

class Launcher(wx.App):
  def OnInit(self):
    self.username = ""
    self.hostname = "localhost"
    self.OverFrame = LauncherFrame(self, None, -1, "")
    self.OverFrame.Show(True)
    self.SetTopWindow(self.OverFrame)
    return True

if __name__ == "__main__":
  num_args = len(sys.argv) - 1
  if not num_args:
      app = Launcher()
      kill_window = partial(app.OverFrame.Show, False)
      app.MainLoop()
      print kill_window
  elif num_args == 1:
    launch(True, sys.argv[1], "localhost")
  elif num_args == 2:
    launch(False, sys.argv[1], sys.argv[2])
