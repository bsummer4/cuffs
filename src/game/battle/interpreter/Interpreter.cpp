#include "Interpreter.hpp"

int eventParser(string &event, vector<string> result){
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

void SimpleInterpreter::handleEvent(string &event){
//  char* mystr = strcpy(event.c_str()); //may have to free
//  vector<char*> Msg = eventParser(event);
  cout << event << endl; //it's so simple it's painful
}

void GameInterpreter::handleEvent(string &event){
    cout << "That's all for now\n" <<endl;
}

int main(){
  SimpleInterpreter si;
  string event = "123412341234 \\MSG these are parameters";
  si.handleEvent(event);
}
