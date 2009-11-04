#pragma once

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>


/**
 * Threaded class. Inherit from this class to get threading functionality.
 * * Idea taken (stolen) from http://blog.emptycrate.com/node/277
 */
class Threaded {
public:
  Threaded();
  virtual void Start();
  virtual void Stop();
private:
  virtual void Run() = 0;

private:
  boost::shared_ptr<boost::thread> runThread;
protected:
  volatile bool stopRequested;
};
