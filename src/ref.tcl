#!/usr/bin/tclsh8.5

set doc {
    # Usage

    ./ref.tcl players [annotation_file]

    example: ./ref 'ben steve'

    The referee will:
    - Wait for all clients to "/identify $name" themselves.
    - Starts the game with '/start'
    - Place all the player objects in their initial locations:

        0 /new player-$name player $x $y

    - Record all /annotate messages }

set SRCDIR /usr/local/bin
source "$SRCDIR/lib.tcl"
source "$SRCDIR/map_parse.xotcl"

# TODO This needs to be passed in at "compile-time" by autotools.
set DATADIR /usr/local/share/fistacuffs

proc rand_x {} { random_from_range 0 8000 }
proc rand_y {} { random_from_range 40 120 }
proc log {obj} { puts stderr $obj; flush stderr; return $obj }
proc change_gravity_wind {gamemap} {
    bind {gravity wind} [random_choice [$gamemap set gravity_wind_pairs]]
    this "{/set global wind $wind} {/set global gravity $gravity}" }
proc place_player {player gamemap} {
    bind {x y} [random_choice [$gamemap set spawnpoints]]
    this "/new player-$player player $x $y" }

proc go {address_player gamemap} {
    puts [log "/map [$gamemap set mapname]"]
    puts [log "/start [map {x {join $x :}} [dict_pairs $address_player]]"]
    set messages [concat [map "x {place_player \$x $gamemap}" \
                              [dict values $address_player]] \
                         [change_gravity_wind $gamemap]]
    set remain [llength $messages]
    foreach message $messages {
        incr remain -1
        puts [log "0.$remain $message"] }}

proc wait_for_players {players} {
    # TODO Add a timeout to wait for people to join
    set address_player [dict create]
    loop {
        gets stdin line
        # ignore emtpy messages and messages from the switchbox
        if {[llength $line] <= 1 || [lindex $line 0] == -1} continue
        if {[llength $line] != 3 || [lindex $line 1] != "/identify"} {
            error "bad /identify line:\n  $line" }
        bind {address command player} $line
        flush stdout
        if {[lexists? $players $player]} {
            dict set address_player $address $player }
        if {[llength $players] == [dict size $address_player]} break }
    return $address_player }

# TODO HACK we use 0.0 for a timestamp because we're too lazy to keep
# track of the current time.
set count 1
proc handle_line {line logfile gamemap} {
    global count
    incr count
    if {$count % 1000 == 0} {
        log {changing it up with wind/gravity}
        foreach output_line [change_gravity_wind $gamemap] {
            puts [log "0.0 $output_line"] }}
    if {[llength $line] < 3} return
    lbind {from timestamp command} [lrange $line 0 2]
    set args [lrange $line 3 end]
    log [list $from $timestamp $command]
    switch $command {
        /annotate {
            lappend annotations $line
            puts $logfile $line
            log $line }
        /gameover { exit 0 }
        default {
            if {$from == -1} {
                puts [log {0.0 /gameover}] }}}}

proc main {players {annotation_file /dev/null}} {
    global DATADIR
    set annotation_file [open $annotation_file w+]
    cd $DATADIR
    set maps [Maps new ./maps]
    if [lempty? [$maps set maps]] { error "No Map files!" }
    set gamemap [dict_random_choice [$maps set maps]]
    go [wait_for_players $players] $gamemap

    # TODO We will need to send messages to keep the syncronizer
    #      running.  This large number hack (1000000.0 hihihi) works
    #      fine for now.
    flush stdout
    puts {1000000.0 hihihi}
    flush stdout
    set annotations [list]
    loop {
        gets stdin line
        if [eof stdin] break
        if [lempty? $line] continue
        handle_line [log $line] $annotation_file $gamemap
        flush stdout }
    close $annotation_file }

apply main $argv
