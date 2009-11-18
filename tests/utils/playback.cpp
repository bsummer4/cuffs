#include "ConnectionManager.hpp"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

ConnectionManager* cm;

void print_usage(char* argv[]){
    cout << "usage: " << argv[0] << " switchbox_hostname switchbox_port script_file" << endl;
}

/**
 * Handles special commands of adding/removing clients
 */
bool handle_special_command(char* buf){
    int client_num = -1;
    cout << "Handling message: " << buf << endl;
    char command[512];
    sscanf(buf, "%s %i", command, &client_num);
    if ( strstr(command, "new_connection") != NULL ){
        cm->addConnection(client_num);
    } else {
        cm->removeConnection(client_num);
    }
}

int main(int argc, char* argv[]){
    if ( argc != 4 ){
        print_usage(argv);
        exit(1);
    }

    cm = new ConnectionManager(std::string(argv[1]), atoi(argv[2]));

    char buf[512];
    char buf2[512];
    FILE* script = fopen(argv[3], "r");
    if ( script == NULL ){
        cout << "Could not open file " << argv[3] << endl;
        print_usage(argv);
        exit(1);
    }


    int client_num;
    while (fscanf(script, "%i", &client_num) == 1){
        fgets(buf2, 511, script);
        //cout << client_num << endl << buf2 << endl;
        if(client_num == -1){
            handle_special_command(buf2);
        } else{
            cout << "sending message: " << client_num << " : " << buf2 << endl;
            cm->sendMessage(client_num, buf2, strlen(buf2));
        }
    }
    sleep(1);
    cout << "Press enter when done." << endl;
    std::string foo;
    cin >> foo;
    return 0;
}
