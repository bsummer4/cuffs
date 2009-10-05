/*
Chris Craig
This code generates random states within the defined
protocol and prints to standard out.

Version 2
*/
/*
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;
*/
#include "stateGen.hpp"

//Useless really
int time_diff(long end, long now){
  return end-now;
}

//Code ripped straight from "C++:Reference"
void wait ( int seconds ){
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

//TODO What defines a "Timestamp"
//ANSWER: for now, its just rawtime
int main(int argc, char **argv){ //legacy code. It isn't necessary
  string s;
  time_t rawtime;
  time(&rawtime);
  
  while(1){
    wait(1);
    printf("%lo\n",(int)rawtime+(clock()/10000));
  }
  return(0);
}
