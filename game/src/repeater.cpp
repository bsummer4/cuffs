/// This opens a connection to two switchboxes and repeats things it
/// hears from the private switchbox onto the public one.  This goes
/// on until it receives a message with 'lost_connection' somewhere in
/// a message.

#include <iostream>
#include <assert.h>
#include "switchbox.hpp"
#include "misc.hpp"

using namespace std;
using namespace misc;
using namespace switchbox;

struct Ignore { void handleEvent(string event) {}};

/// Exit the program when we are told to.  Otherwise return true.  This
/// is meant to be used as a message filter.
bool die (string event) {
    SBMessage* message = destringify(event);
    if (strstr(message->data, "lost_connection"))
        exit(0);
    return true; };

int main(int num_args, char **args) {
    assert ("Usage: repeater listen-hostname port send-hostname port" &&
            num_args == 5);
    Ignore ignore;
    Connection <typeof(ignore)&> out(string(args[3]),
                                     atoi(args[4]),
                                     ignore);
    Filter <typeof(out)&> die_when_told(out, die);
    Connection <typeof(die_when_told)&> in(string(args[1]), atoi(args[2]),
                                           die_when_told);
    in.start();
    out.run();
    return 1; }
