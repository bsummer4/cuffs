from switchbox_client import *
import sys

class Connection:
    def __init__(self, hostname, port):
        "Opens the connection.  "
        self.hostname = hostname
        self.id = -1 # we don't know yet.  We'll set this when we get
                     # our first message.
        self.port = port
        self.connection = self.open(hostname, port)
    def working(self):
        return self.connection and valid_socket(self.connection)
    def close(self):
        if self.working():
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
        if not self.working(): raise Exception("Connection is closed.  ")
        message = string_to_message(routing_type, self.id, target_id, string)
        success = switchbox_send_safe (self.connection, message)
        if not success:
            self.close()
            raise Exception("Couldn't send.  Lost connection to server.  ")
    def receive(self):
        if not self.working(): raise Exception("Connection is closed.  ")
        message = switchbox_receive_safe(self.connection)
        if message == None:
            self.close()
            raise Exception("Couldn't receive.  Lost connection to server.  ")
        if (message_to(message) == message_from(message)):
            self.id = message_to(message)
        if message_type(message) == 3: return self.receive()
        return message
