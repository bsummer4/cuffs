#include "Generator.hpp"
#include "Connection.hpp"
#include <iostream>
#include "switchbox_client.h"
#define MAXLEN 512 

//TODO Read timestamps from Sync
//ANSWER: for now, its just rawtime
int main(int argc, char** argv)
{
  Generator g = Generator();
  g.debug = false; 
  time_t rawtime;
  srand(time (NULL));
  SBMessage* message = NULL;
  string hostname("localhost");
  Connection c = Connection(hostname.c_str(), 80044);
  char buffer[MAXLEN];
  int i;
  for (i=0;i<10;i++)
  {
    time (&rawtime);
    g.genStateMsg(buffer, MAXLEN, (int)rawtime);
    cout << buffer << endl;
    sleep(1); //Assumed it will wait 1/2 a sec untill the next message
  }
  return 0;
}
