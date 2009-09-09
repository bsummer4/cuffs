from switchbox_client import *

class Connection:
    def __init__(self, hostname, port):
        "Opens the connection.  "
        self.hostname = hostname
        self.id = -1 # we don't know
        self.port = port
        self.connection = self.open(hostname, port)
    def close(self):
        if self.connection and valid_socket(self.connection):
            close_connection(self.connection)
        self.connection = None
    def open(self, hostname, port):
        connection = open_connection(hostname, port);
        if not valid_socket(connection):
            raise Exception("Failed to connect to server %s:%d"%(
                    hostname, port));
        return connection
    def broadcast(self, string): return self.send(-1, string, 1)
    def send(self, target_id, string, routing_type=0):
        assert(valid_socket(self.connection))
        success = switchbox_send_string(
            self.connection,
            len(string)+16, routing_type, self.id, target_id,
            string)
        if not success: raise Exception("Failed to send.  ")
    def receive(self):
        assert(valid_socket(self.connection))
        sizep, typep, fromp, top = [intp() for x in range(4)]
        result = switchbox_receive_string_safe (
            self.connection,
            sizep, typep, fromp, top)
        source, target = [p.value() for p in (fromp, top)]
        if (source == target): self.id = source
        return result
