#!/usr/bin/env tclsh8.5

source lib.tcl
set underworld_port 5151
set overworld_port 38235

proc launch {run_server username hostname} {
    global overworld_port underworld_port
    set connect "switchbox-connect $hostname $overworld_port"
    set client "overworld-client.py $username $hostname"
    if $run_server {
        if [catch {exec switchbox.sh start $overworld_port &}] {
            exec switchbox.sh stop $overworld_port
            error {Unable to start switchbox} }
        if [catch {exec server.sh start &}] {
            exec switchbox.sh stop $overworld_port
            error {Unable to start overworld-server} }}
    catch {exec sixty-nine $connect $client}
    if $run_server {
        catch {exec server.sh stop}
        catch {exec switchbox.sh stop $overworld_port} }}

proc main {args} {
    global argv0
    switch [llength $args] {
        1 { launch 1 [lindex $args 0] localhost }
        2 { launch 0 [lindex $args 0] [lindex $args 1] }
        default { puts stderr "usage: $argv0 username \[hostname\]" }}}

apply main $argv
