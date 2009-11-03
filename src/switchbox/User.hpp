#include "Connection.hpp"
#include "SwitchboxAdmin.hpp"
#include "switchbox_client.h"

//Prototypes
class User {
    int u_id;
    int group;
    bool admin;

public:
    User(int id) {
        this.u_id = id;
    }
    ///@TODO write next_free_group;



