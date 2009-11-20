#include "ConnectionManager.hpp"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
extern "C"{
#include <assert.h>
}

using namespace std;

ConnectionManager* cm;

void print_usage(char* argv[]){
    cout << "usage: " << argv[0] << " switchbox_hostname switchbox_port [script_file]" << endl;
}

/**
 * Handles special commands of adding/removing clients
 */
bool handle_special_command(char* buf){
    int client_num = -1;
    cout << "Handling message: " << buf << endl;
    char command[512];
    assert( 2 == sscanf(buf, "%s %i", command, &client_num));

    // Special case client_num 0, he is explicitly there
    if ( client_num == 0 ){
        cerr << "Got a command to add/remove client 0, but it is explicitly there. Ignoring..." << endl;
        return true;
    }

    if ( strstr(command, "new_connection") != NULL ){
        assert(cm->addConnection(client_num));
    } else {
        assert(cm->removeConnection(client_num));
    }
    return true;
}

int main(int argc, char* argv[]){
    char buf[512];
    char buf2[512];
    FILE* script;

    if ( argc != 4 and argc != 3 ){
        print_usage(argv);
        exit(1);
    }

    cm = new ConnectionManager(std::string(argv[1]), atoi(argv[2]));

    if ( argc == 3 ){
        script = stdin;
    } else {
        script = fopen(argv[3], "r");
    }
    if ( script == NULL ){
        cout << "Could not open file " << argv[3] << endl;
        print_usage(argv);
        exit(1);
    }


    int client_num;
    while (fscanf(script, "%i", &client_num) == 1){
        fgets(buf2, 511, script);
        cerr << client_num << endl << buf2 << endl;
        if(client_num == -1){
            assert(handle_special_command(buf2));
        } else{
            cerr << "sending message: " << client_num << " : " << buf2 << endl;
            assert(cm->sendMessage(client_num, buf2, strlen(buf2)));
        }
    }
    return 0;
}
