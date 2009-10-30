#!/usr/bin/python
import os
import sys
import subprocess
import time
import signal
from threading import Thread

retval = 0

class switchbox_manager():
    def __init__(self):
        self.pid = -1
        self.switchbox_path="../../src/switchbox/switchbox"
    def start_switchbox(self):
        self.pid = os.fork()
        if (self.pid == 0):
            print "Starting Switchbox..."
            sys.stdout = open("switchbox-output.txt", 'w')
            sys.stderr = sys.stdout
            # Check to see if valgrind is available
            str = os.popen('which valgrind').read()
            if ( len(str) > 0 ):
                # We have valgrind
                os.execlp("valgrind", "valgrind", "-q", "--log-fd=1", self.switchbox_path)
            else: 
                # We don't have valgrind
                os.execlp(self.switchbox_path, self.switchbox_path)
            # Shouldn't get here
            os._exit(0)

    def stop_switchbox(self):
        if (self.pid > 0):
            os.kill(self.pid,signal.SIGKILL)
            os.waitpid(self.pid,0)

        if ( os.path.getsize('switchbox-output.txt') == 0 ):
            return True
        else : 
            return False


def call_sys(testname):
    run = testname.rsplit()
    print run
    retval = 0
    retval = subprocess.call(run)
    return
            

def run_test(testname, timeout_sec):
    worker = Thread(target=call_sys, args=[testname])
    worker.start()

    worker.join(timeout_sec)

    if ( worker.isAlive() ):
        # The thread timed out. 
        # Python threads can't be canceled... so uhhh lets just fail loudly and exit?
        print "error: Test timed out!"
        return False
    else :
        return True

def cleanup_sys(sm):
    print "Failed previous test, can not continue..."
    sm.stop_switchbox()
    exit(1);


def main():

    sm = switchbox_manager()
    sm.start_switchbox()
    time.sleep(1)

    if ( False == run_test("./function_tests",10) or retval != 0):
        return cleanup_sys(sm);
    print "Function Tests sucessfully completed\n"

    if ( False == run_test("./edgecase_tests", 10) or retval != 0):
        return cleanup_sys(sm);
    print "Edge Case Tests sucessfully completed\n"

    print "\nRunning Latency/Scalability Tests"
    if ( False == run_test("./latency_test 50", 240) or retval != 0):
        return cleanup_sys(sm);
    print "Latency/Scalability Tests Completed"

    print "Killing Switchbox"
    if ( False == sm.stop_switchbox() ):
        print 'Switchbox encountered errors. see switchbox-output.txt for more details'
        return 1
    print "Done"

    return 1


if __name__ == "__main__": main()
