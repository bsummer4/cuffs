#include <Connection.hpp>
#include <iostream>

static const int TIMEOUT = 240000; // 4 Minutes

using namespace std;

void print_usage(char* binname){
    cout << "Usage: " << binname << " main_switchbox_hostname main_switchbox_port "
         << "private_switchbox_hostname private_switchbox_port key" << endl;
}

int main(int argc, char * argv[]){
    if ( argc != 6 ){
        print_usage(argv[0]);
        exit(1);
    }
    char * main_switchbox_hostname = argv[1];
    int main_switchbox_port = atoi(argv[2]);
    char * private_switchbox_hostname = argv[3];
    int private_switchbox_port = atoi(argv[4]);
    int key = atoi(argv[5]);

    if ( main_switchbox_port == 0 || private_switchbox_port == 0 || key == 0){
        print_usage(argv[0]);
        exit(1);
    }

    Connection in(private_switchbox_hostname, private_switchbox_port);
    Connection out(main_switchbox_hostname, main_switchbox_port);

    in.start();
    out.start();

    while(1){
        in.blockForMessage(TIMEOUT);
        SBMessage* msg = in.getMessage();
        if ( !msg || strstr(msg->data,"lost_connection") != NULL )
            return 0;
        out.sendMessage(msg);
    }

    return 0;
}
