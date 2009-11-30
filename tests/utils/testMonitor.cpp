#include <Connection.hpp>
#include "ConnectionManager.hpp"
#include <iostream>

static const int TIMEOUT = 240000; // 4 Minutes

using namespace std;

void print_usage(char* binname){
    cout << "Usage: " << binname << " main_switchbox_hostname"
         << " main_switchbox_port " << endl;
}

int main(int argc, char * argv[]){
    if ( argc != 3 ){
        print_usage(argv[0]);
        exit(1);
    }
    char * main_switchbox_hostname = argv[1];
    int main_switchbox_port = atoi(argv[2]);

    if ( main_switchbox_port == 0 ){
        print_usage(argv[0]);
        exit(1);
    }

    MonitorConnection in(main_switchbox_hostname, main_switchbox_port);

    in.start();

    while(1){
        in.blockForMessage(TIMEOUT);
        SBMessage* msg = in.getMessage();
        cout << msg->data << endl;
    }

    return 0;
}
