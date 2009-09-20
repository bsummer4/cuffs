#!/usr/bin/python
import os
import subprocess
import time
import signal


class switchbox_manager():
    def __init__(self):
        self.pid = -1
        self.switchbox_path="../../src/switchbox/switchbox"
    def start_switchbox(self):
        self.pid = os.fork()
        if (self.pid == 0):
            print "Starting Switchbox..."
            #os.system(self.switchbox_path)
            os.execl(self.switchbox_path, self.switchbox_path)
            os._exit(0)

    def stop_switchbox(self):
        if (self.pid > 0):
            os.kill(self.pid,signal.SIGKILL)
            os.waitpid(self.pid,0)
            




def main():

    sm = switchbox_manager()
    sm.start_switchbox()

    #print "Do Stuff here"
    fval = os.system("./function_tests")
    if ( fval != 0 ):
        return fval


    print "Killing"
    sm.stop_switchbox()
    print "Done"


if __name__ == "__main__": main()
