#include "ConnectionManager.hpp"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <string>
extern "C"{
#include <assert.h>
}

using namespace std;
bool private_switchbox_running = false;

char * public_switchbox_address;
int public_switchbox_port;
char private_switchbox_address[1024];
int private_switchbox_port = 3214;

RemoteConnectionManager* cm;

void print_usage(char* argv[]){
    cout << "usage: " << argv[0] << " switchbox_hostname switchbox_port "
         << "[script_file]" << endl;
}

/**
 * Handles special commands of adding/removing clients
 */
bool handle_special_command(char* buf){
    int client_num = -1;
    cout << "Handling message: " << buf << endl;
    string command;
    //assert( 2 == sscanf(buf, "%s %i", command, &client_num));
    cin >> command;


    // Special case client_num 0, he is explicitly there
    if ( client_num == 0 ){
        cerr << "Got a command to add/remove client 0, but it is explicitly"
             << "there. Ignoring..." << endl;
        return true;
    }

    if ( command.find("new_connection") != string::npos ){
        cin >> client_num;
        assert(!cin.fail());
        assert(client_num>0);
        assert(cm->addConnection(client_num));
    } else if ( command.find("new_remote_connection") != string::npos ){
        string hostname;
        cin >> hostname >> client_num;
        cm->addRemoteConnection(client_num,hostname);
    }
    else {
        cin >> client_num;
        assert(!cin.fail());
        assert(client_num>0);
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

    public_switchbox_address = argv[1];
    public_switchbox_port = atoi(argv[2]);

    // Determine my system's hostname
    assert(gethostname(private_switchbox_address,512));

    cm = new RemoteConnectionManager(public_switchbox_address, public_switchbox_port);

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
    int fscanf_ret_val;
    
    while (true){
        if ((fscanf_ret_val = fscanf(script, "%i", &client_num)) != 1){
            // Check if it failed because we got an eof
            if ( fscanf_ret_val == EOF ) return 0;
            cerr << "Could not get client number" << endl;
            // Read the line and throw it out because it's invalid.
            fgets(buf, 511, script);
            continue;
        }

        fgets(buf2, 511, script);
        //cerr << client_num << endl << buf2 << endl;
        if(client_num == -1){
            assert(handle_special_command(buf2));
        } else{
            //cerr << "sending message: " << client_num << " : " << buf2 << endl;
            if(!cm->sendMessage(client_num, buf2, strlen(buf2))){
                cerr << "Error Message not sent. Bad client number: " 
                     << client_num << endl;
            }
        }
    }
    return 0;
}
