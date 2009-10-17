#include "Interpreter.hpp"

int eventParser(string &event, vector<string> &result){
  string::size_type start;
  string::size_type end;
  end = event.find_first_of(' '); 
  start = 0;

  while(end != string::npos){
    result.push_back(event.substr(start,end));
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

SimpleInterpreter::SimpleInterpreter(){};

void SimpleInterpreter::handleEvent(string &event){
  int timestamp;
  vector<string> result;
  eventParser(event, result);
//  cout << result[0] << endl;
    vector<string>::iterator v_it = result.begin();
      vector<string>state_change;
//      cout << result[0] << endl;

      timestamp = stringtoint(result[0]); //


      for(v_it = result.begin()+1; v_it != result.end(); v_it++){
        state_change.push_back(*v_it);
      }
      cout << "Timestamp should be: " << result[0] << endl;
      cout << "Timestamp is: " << timestamp << endl;

      cout << "State Changes: \n" << endl; 
      for(v_it = state_change.begin(); v_it != state_change.end();v_it++){
        cout << *v_it << endl;
      }
}

GameInterpreter::GameInterpreter(){};
void GameInterpreter::handleEvent(string &event){
  cout << "That's all for now\n" <<endl;
}

int main(){
  SimpleInterpreter si;
  string event = "123412341234 \\MSG these are parameters";
  si.handleEvent(event);
}
