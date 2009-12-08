#!/usr/bin/python

import wx, sys, threading, os, time

underworld_port = 5151
overworld_port = 38235

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
    if self.servercheck.IsChecked():
      if not os.system("./switchbox.sh start %d"%overworld_port):
        raise "Unable to start switchbox"
      if not os.system("./server.sh"):
        os.system("./switchbox.sh stop %d"%overworld_port)
        raise "Unable to start overworld-server"

    connect = "'./switchbox-connect %s %s'"%(self.launcher.hostname,
                                           underworld_port)
    runclient = './overworld-client.py %s %s'%(
      self.launcher.username, self.launcher.hostname)
    os.system("./sixty-nine %s %s"%(connect, runclient))

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
  app = Launcher()
  app.MainLoop()
