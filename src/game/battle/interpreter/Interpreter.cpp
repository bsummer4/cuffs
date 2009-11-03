#include "Interpreter.hpp"
using namespace std;
using namespace GameUtils;

int eventParser(std::string &event, vector<std::string> &token) {
    string::size_type start;
    string::size_type end;
    end = event.find_first_of(' ');
    start = 0;

    while (end != string::npos) {
        token.push_back(event.substr(start+1,end-start - 1));
        start = end;
        end = event.find_first_of(' ', start+1);
    }
    token.push_back(event.substr(start+1, event.length()-start-1));
    return token.size();
}

int stringtoint(std::string str) {
    int ret = 0;
    stringstream SStream(str);
    SStream >> ret;
    return ret;
}

float stringtofloat(std::string str) {
    float ret = 0;
    stringstream SStream(str);
    SStream >> ret;
    return ret;
}

void CatInterpreter::handleEvent(std::string &event) {
    cout << event << endl;
}

SimpleInterpreter::SimpleInterpreter() {};

void SimpleInterpreter::handleEvent(std::string &event) {
    int timestamp;
    vector<string> token;
    eventParser(event, token);
    vector<string>::iterator v_it = token.begin();
    vector<string>state_change;

    timestamp = stringtoint(token[0]); //

    for (v_it = token.begin()+1; v_it != token.end(); v_it++) {
        state_change.push_back(*v_it);
    }
    cout << "Message: " << event << endl;
    cout << "Timestamp is: " << timestamp << endl;

    cout << "State Changes:" << endl;
    for (v_it = state_change.begin(); v_it != state_change.end();v_it++) {
        cout << *v_it << endl;
    }
}

GameInterpreter::GameInterpreter(State &gameState) : state(gameState) { };

void GameInterpreter::handleEvent(std::string &event) {
    Coord coord;
    int xvel;
    int yvel;
    int timestamp;
    int command;
    vector<string> token;
    eventParser(event, token);
    vector<string>::iterator v_it = token.begin();
    vector<string>state_change;

    cout << event << endl;
    cout << token[1] << endl;
    if (token.size() < 2) {
    } else {
        timestamp = stringtoint(token[0]);
        command = GetCommand(token[1]);
        switch (command) {
        case MAP:
            if (token.size() < 3) {
                cerr << "Malformed command sent to interpreter.  /map must be followed by a valid map name string" << endl;
                return;
            }
            state.setMap(token[2]);
            break;
        case SHOOT:

            if (token.size() < 6) {
                cerr << "Malformed command sent to interpreter.  /shoot must be followed by user_id, angle, power, weaponid, and projectile-id" << endl;
                return;
            }
            coord = state.getPlayerLocation(token[2]);
            xvel = stringtofloat(token[4]) * cos(stringtofloat(token[3]));
            yvel = stringtofloat(token[4]) * sin(stringtofloat(token[3]));
            state.addProjectile(token[6], token[5], coord, xvel, yvel);

            //FIGURE OUT HOW TO SHOOT
            break;
        case BATTLESTART:
            state.startBattle();
            break;
        case BATTLESTOP:
            state.stopBattle();
            break;
        case WEAPON:
            if (token.size() < 3) {
                cerr << "Malformed command sent to interpreter.  /weapon must be followed by a valid integer weaponid" << endl;
                return;
            }
            state.changeWeapon(token[2]);
            break;
        case MOVE:
            if (token.size() < 5) {
                cerr << "Malformed command sent to interpreter.  /move must be followed by a valid integer obj_id, x, and y" << endl;
                return;
            }
            state.moveObj(token[2], stringtoint(token[3]), stringtoint(token[4]));
            break;
        case HIT:
            if (token.size() < 5) {
                cerr << "Malformed command sent to interpreter.  /hit must be followed by a valid integer obj_id, x, and y" << endl;
                return;
            }
            state.hitObj(token[2], stringtoint(token[3]), stringtoint(token[4]));
            break;
        case QUERY:
            if (token.size() < 4)
            {
                cerr << "Malformed command sent to interpreter.  /query must be followed by a valid integer obj_id, x, and y" << endl;
                return;
            }
            //WHATEVER WE QUERY???
            break;
        }
    }
    return;
}
