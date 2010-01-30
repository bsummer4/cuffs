#!/usr/bin/env tclsh8.5

set doc {
    Usage: overworld-server.tcl

    Maintains a list of players currently on the switchbox interfaces
    with the rest of the game using stdin and stdout }

source lib.tcl
set playerlist []
set game_port 5151

loop {
    gets stdin line
    if [eof stdin] break
    if {[lempty? line] || [llength $line] < 2} continue
    lbind {sender command} [lrange $line 0 1]
    set args [lrange $line 2 end]
    switch $command {
        /login { lappend playerlist [lindex $args 0] }
        /logout { set playerlist [lwithout $playerlist [lindex $args 0]] }
        /list { puts "/players [join $playerlist { }]" }
        /play {
            do "switchbox.sh start $game_port > /dev/stderr"
            puts stderr {starting ref}
            do "sixty-nine 'ref.tcl $args' 'switchbox-connect localhost $game_port'"
            puts stderr {ref finished; stopping switchox}
            do "switchbox.sh stop $game_port > /dev/stderr"
            puts stderr {switchbox stoped} }
        /quit { exit }}
    flush stdout }
