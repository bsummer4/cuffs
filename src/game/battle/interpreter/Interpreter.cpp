#include "Interpreter.hpp"


string stringParser(string &event){
  char* tmp = strtok(event.c_str(), ' ');
  sscanf(tmp, "%d");
  char* tmp = strtok(event.c_str(), ' ');
    while( tmp != NULL ) {
      printf( "result is \"%s\"\n", result );
      result = strtok( NULL, delims );
    }
}


void SimpleInterpreter::HandleEvent(string &event){
  stringParser(event)
  cout << event << endl; //it's so simple it's painful
}

void GameInterpreter::HandleEvent(string &event){
    cout << "That's all for now\n" <<endl;
}
