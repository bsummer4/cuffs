from switchbox_client import *

class Connection:
    def __init__(self, hostname, port):
        connection = open_connection(hostname, port);
        if not valid_socket(connection):
            raise Exception("Failed to connect to server %s:%d"%(
                    hostname, port));
        self.connection = connection
        self.hostname = hostname
        self.id = -1 # we don't know
        self.port = port
    def broadcast(self, string): return self.send(-1, string, 1)
    def send(self, target_id, string, routing_type=0):
        success = switchbox_send_string(
            self.connection,
            len(string)+16, routing_type, self.id, target_id,
            string)
        if not success: raise Exception("Failed to send.  ")
    def receive(self):
        sizep, typep, fromp, top = [intp() for x in range(4)]
        result = switchbox_receive_string_safe (
            self.connection,
            sizep, typep, fromp, top)
        source, target = [p.value() for p in (fromp, top)]
        if (source == target): self.id = source
        return result
