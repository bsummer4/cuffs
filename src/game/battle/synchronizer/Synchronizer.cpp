#include "Syncronizer.hpp"

Synchronizer::Synchronizer(Connection * _con, Interpreter * _interpreter){
    con = _con;
    interpreter = _interpreter;
}

unsigned int Synchronizer::currentTime(){
    boost::mutex::scoped_lock l(timeLock);
    return cTime;
}
