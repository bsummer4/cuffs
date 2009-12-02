#!/usr/bin/python

import wx, sys, threading, os

underworld_port = 5151

# Launcher GUI Frame
class LauncherFrame(wx.Frame):
  def __init__(self, launcher, *args, **kwds, size=(600,400)):
    kwds["style"] = wx.DEFAULT_FRAME_STYLE
    wx.Frame.__init__(self, *args, **kwds)
    self.userLabel = wx.StaticText(self, -1, 'Username:', name='userlabel')
    self.userBox = wx.TextCtrl(self,-1, name='user')
    self.hostLabel = wx.StaticText(self, -1, 'Hostname:', name='hostlabel')
    self.hostBox = wx.TextCtrl(self,-1, name='host')
    self.hostBox.SetValue('localhost')
    self.launcher = launcher

    self.__set_properties()
    self.__do_layout()

class Launcher(wx.App):
  def OnInit(self):
    self.OverFrame = LauncherFrame(self, None, -1, "")
    self.OverFrame.Show(True)
    self.SetTopWindow(self.OverFrame)
    return True

if __name__ == "__main__":
  app = Launcher()
  app.MainLoop()
