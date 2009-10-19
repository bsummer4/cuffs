#include "Interpreter.hpp"

int eventParser(string &event, vector<string> &token){
  string::size_type start;
  string::size_type end;
  end = event.find_first_of(' '); 
  start = 0;

  while(end != string::npos){
    cout << "test" << endl;
    token.push_back(event.substr(start,end-start));
    start = end;
    end = event.find_first_of(' ', start+1); 
  }
  return token.size();
}

int stringtoint(string str){
  int ret = 0;
  stringstream SStream(str);
  SStream >> ret;
  return ret;
}

void CatInterpreter::handleEvent(string &event){
    cout << event << endl;
}

SimpleInterpreter::SimpleInterpreter(){};

void SimpleInterpreter::handleEvent(string &event){
  int timestamp;
  vector<string> token;
  eventParser(event, token);
  vector<string>::iterator v_it = token.begin();
  vector<string>state_change;

  timestamp = stringtoint(token[0]); //

  for(v_it = token.begin()+1; v_it != token.end(); v_it++){
    state_change.push_back(*v_it);
  }
  cout << "Message: " << event << endl; 
  cout << "Timestamp is: " << timestamp << endl;

  cout << "State Changes:" << endl; 
  for(v_it = state_change.begin(); v_it != state_change.end();v_it++){
    cout << *v_it << endl;
  }
}

GameInterpreter::GameInterpreter(State &gameState) : state(gameState) { };

void GameInterpreter::handleEvent(string &event) {
  int timestamp;
  int command;
  vector<string> token;
  eventParser(event, token);
  vector<string>::iterator v_it = token.begin();
  vector<string>state_change;

  if(token.size() < 2) {
  } else {
    timestamp = stringtoint(token[0]); 
    command = GetCommand(token[1]); 
    switch(command) {
      case MAP:
        if(token.size() < 3) {
          cerr << "Malformed command sent to interpreter.  /map must be followed by a valid map name string" << endl;
          return;
        }
        state.setMap(token[2]);
        break;
      case SHOOT:
        if(token.size() < 5) {
          cerr << "Malformed command sent to interpreter.  /shoot must be followed by user_id, angle, power, and weaponid" << endl;
          return;
        }
        //FIGURE OUT HOW TO SHOOT
        break;
      case BATTLESTART:
        state.startBattle();
        break;
      case BATTLESTOP:
        state.stopBattle();
        break;
      case WEAPON:
        if(token.size() < 3) {
          cerr << "Malformed command sent to interpreter.  /weapon must be followed by a valid integer weaponid" << endl;
          return;
        }
        state.changeWeapon(stringtoint(token[2]));
        break;
    }
  }
}
