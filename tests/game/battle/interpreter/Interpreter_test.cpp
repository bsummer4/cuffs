#include "Interpreter.hpp"
#include "State.hpp"

int main(int argc, char **argv) {
  SimpleInterpreter si;
  string event = "12341234 /MSG these are parameters";
  si.handleEvent(event);

  cout << "HardCoded events:" << endl;
  State *state = new State();
  GameInterpreter gi(*state);
  event = "12341234 /map mapname";
  gi.handleEvent(event);
  event = "12341234 /weapon 3";
  gi.handleEvent(event);
  event = "12341234 /battlestart";
  gi.handleEvent(event);
  event = "12341234 /battlestop mapname";
  gi.handleEvent(event);
  event = "12341234 /shoot mapname";
  
}
