# !!!
# READ THIS!!
# !!!
#
# Zero of this code works.  It's just a brain dump.  Don't try to
# figure it out.

class GameState:
    """This implementation is (very) nieve for simplicity.  We need to
       maintain a data structure of the current game state.  So we
       currently do this by just keeping all the messages (in reverse
       order).  This will not scale, but it's almost no work.  So, I
       will use this until I have working code, then rewrite.  """

    def __init__(self):
        self.messages = []

    def to_messasges(self):
        """Returns a list of messages that can be used to replicate
           this state.  i.e:
             state2=GameState();
             [state2.update(m) for m in state1.to_messages()]
             assert(more_or_less_equal_p(state1, state2))
        """
        return reverse(self.messages)

    def valid_message_p(self, message):
        "Is the message valid given the current state?  "
        return True

    def update(self, message):
        self.messages.append(message)
        return message


state = GameState()
connection = Connection(hostname, port)

def announce(game_message):
    # TODO I think we should just resend the message, but I'm not sure.
    connection.broadcast(game_message)

def new_user_announcement_p(message):
    return message_type(message) == 'announce' and
           announcement_code(message) == 'new_connection'

def login(connection, user):
    for message_str in state.to_messages():
        connection.send(user, message_str)

def main():
    while True:
        message = connection.receive()
        if not valid_message_p(message): continue
        if new_user_announcement_p(message):
            login(connection, message_from(message))
            continue
        announce(state.update(message_data(message)))
