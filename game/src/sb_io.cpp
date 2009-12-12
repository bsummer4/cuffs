/**
@addtogroup sb_io sb_io
@ingroup Game

@section Description
Creates a connection to the switchbox at the given port and sends the messages
as various different clients to the switchbox to facilitate testing of multiple
clients despite having fewer then that. Likewise, it will print everything it 
receives from the switchbox to its stdout. 

It is usually safe to assume, that the input should match the output over the 
switchbox.

@section Usage
@verbatim
    usage: sb_io switchbox_hostname switchbox_port [script_file]
@endverbatim
If the script_file is given, then it will use that, otherwise it will read its
input from stdin.

@section sfile Script File
The script file will look as follows
@verbatim
-1 new_connection 0
-1 new_connection 1
-1 new_connection 2
1 hi
2 whatever
1 That's not very nice!
2 no you!
1 I'm telling the chanOp!
-1 new_connection 3
3 op:  You are mean: *kick*
-1 lost_connection 2
1 yay
3 yay
-1 new_connection 2
2 sup dog?  I like pizza.
-1 lost_connection 1
-1 lost_connection 3
2 whatever...
-1 lost_connection 2
@endverbatim

Where the first "field" is the user to send the message as, and user -1
represents a special message to sb_io to add or remove a virtual client. 
**/
#include "ConnectionManager.hpp"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
extern "C"{
#include <assert.h>
#include <signal.h>
}

using namespace std;
using namespace sb_io;

char * public_switchbox_address;
int public_switchbox_port;

RemoteConnectionManager* cm;

extern "C" void handler(int signum){
    delete cm;
    exit(0);
}

void print_usage(char* argv[]){
    cout << "usage: " << argv[0] << " switchbox_hostname switchbox_port "
         << "[script_file]" << endl;
}

// Returns a pointer to the first element in the string that is not a space
char *trim_front(char *str){
    char * start = str;
    while(*start==' ') start++;
    return start;
}

/**
 * Handles special commands of adding/removing clients
 */
bool handle_special_command(char* buf){
    int client_num = -1;
    istringstream ss;
    ss.str(buf);
    if (CMDEBUG) cerr << "Handling message: " << buf << endl;
    string command;
    ss >> command;
    if (CMDEBUG) cerr << "Command: " << command << endl;


    // Special case client_num 0, he is explicitly there
    if ( client_num == 0 ){
        cerr << "Got a command to add/remove client 0, but it is explicitly"
             << "there. Ignoring..." << endl;
        return true;
    }

    if ( command.find("new_connection") != string::npos ){
        if (CMDEBUG) cerr << "new connection" << endl;
        ss >> client_num;
        assert(!ss.fail());
        assert(client_num>0);
        assert(cm->addConnection(client_num));
    } else if ( command.find("new_remote_connection") != string::npos ){
        if (CMDEBUG) cerr << "new remote connection" << endl;
        string hostname;
        ss >> hostname >> client_num;
        assert(!ss.fail());
        assert(cm->addRemoteConnection(client_num,hostname));
    }
    else if( command.find("lost_connection") != string::npos ){
        if (CMDEBUG) cerr << "lost connection" << endl;
        ss >> client_num;
        assert(!ss.fail());
        assert(client_num>0);
        assert(cm->removeConnection(client_num));
    } else { 
        cerr << "Warning: Did not understand message... skipping" << endl;
    }
    if (CMDEBUG) cerr << "done" << endl;
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
    // Set up a signal handler
    signal(SIGINT, handler);

    public_switchbox_address = argv[1];
    public_switchbox_port = atoi(argv[2]);

    cm = new RemoteConnectionManager(public_switchbox_address, public_switchbox_port);

    if ( argc == 3 ){
        script = stdin;
    } else {
        script = fopen(argv[3], "r");
    }
    if ( script == NULL ){
        cerr << "Could not open file " << argv[3] << endl;
        print_usage(argv);
        exit(1);
    }

    int client_num;
    int fscanf_ret_val;
    
    while (true){
        if ((fscanf_ret_val = fscanf(script, "%i", &client_num)) != 1){
            // Check if it failed because we got an eof
            if ( fscanf_ret_val == EOF ) {
                usleep(100000);
                delete cm;
                return 0;
            }
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
            if(!cm->sendMessage(client_num, trim_front(buf2), strlen(trim_front(buf2)))){
                cerr << "Error Message not sent. Bad client number: " 
                     << client_num << endl;
            }
        }
    }
    return 0;
}
