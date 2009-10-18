#include "Interpreter.hpp"
#include "GetCommand.hpp"
using namespace GameUtils;

int eventParser(string &event, vector<string> &result){
  string::size_type start;
  string::size_type end;
  end = event.find_first_of(' '); 
  start = 0;

  while(end != string::npos){
    cout << "test" << endl;
    result.push_back(event.substr(start,end-start));
    start = end;
    end = event.find_first_of(' ', start+1); 
  }
}

int stringtoint(string str){
  int ret = 0;
  stringstream SStream(str);
  SStream >> ret;
  return ret;
}

CatInterpreter::CatInterpreter(){};

void CatInterpreter::handleEvent(string &event){
    cout << event << endl;
}

SimpleInterpreter::SimpleInterpreter(){};

void SimpleInterpreter::handleEvent(string &event){
  int timestamp;
  vector<string> result;
  eventParser(event, result);
  vector<string>::iterator v_it = result.begin();
  vector<string>state_change;

  timestamp = stringtoint(result[0]); //

  for(v_it = result.begin()+1; v_it != result.end(); v_it++){
    state_change.push_back(*v_it);
  }
  cout << "Message: " << event << endl; 
  cout << "Timestamp is: " << timestamp << endl;

  cout << "State Changes:" << endl; 
  for(v_it = state_change.begin(); v_it != state_change.end();v_it++){
    cout << *v_it << endl;
  }
}

GameInterpreter::GameInterpreter(){};

void GameInterpreter::handleEvent(string &event){
  int timestamp;
  int command;
  vector<string> result;
  eventParser(event, result);
  vector<string>::iterator v_it = result.begin();
  vector<string>state_change;

  if(result.size() < 2) {
  } else {
    timestamp = stringtoint(result[0]); 
    //    command = GetCommand(result[1]); //to be implemented--getCommand is in common/
    switch(command) {
      case MAP:
        break;
      case SHOOT:
        break;
      case BATTLESTART:
        break;
      case BATTLESTOP:
        break;
      case WEAPON:
        break;
    }
  }
}
