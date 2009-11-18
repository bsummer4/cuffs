#include "ConnectionManager.hpp"
#include <iostream>

using namespace std;

/**
 * Recorder works by creating the ConnectionManager with one client to the switchbox, it will then 
 * record everything that it gets out to a file. 
 */

void print_usage(char* argv[]){
    cout << "usage: " << argv[0] << " switchbox_hostname switchbox_port" << endl;
}

int main(int argc, char* argv[]){
    if ( argc != 3 ){
        print_usage(argv);
        exit(1);
    }
    ConnectionManager cm(argv[1], atoi(argv[2]), true);
    cm.addConnection(1);
    cout << "Press enter to stop recording." << endl;
    std::string foo;
    cin >> foo;
    return 0;
}
