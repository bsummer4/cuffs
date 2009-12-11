#pragma once
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

class Threaded {
public:
  Threaded() : stopRequested(false), runThread() {}
  virtual void start() {
    assert(!runThread);
    runThread = boost::shared_ptr<boost::thread>
      (new boost::thread
       (boost::bind(&Threaded::run, this)));}
  virtual void stop() {
    assert(runThread);
    stopRequested = true;
    runThread->join();}
  void join() { runThread->join(); }
protected:
  volatile bool stopRequested;
private:
  virtual void run() = 0;
  boost::shared_ptr<boost::thread> runThread; };
