proc list_with list {
    for {set i 0} {$i < [llength $list]} {incr i} {
        uplevel "set $i [lindex $list $i]" }}

proc appendl args {
    set result [list]
    foreach arg $args {
        foreach elt $arg {
            lappend result $elt }}
    return $result }

proc circle {radius pos color} {
    eval [appendl circ $radius $color $pos] }

proc every {ms command} {
    uplevel #0 $command
    after $ms [list every $ms $command] }

# Interprets a line that might be an incomplete input.
set buf {}
proc interpret line {
	append ::buf $line "\n"
	if {[info complete $::buf]} {
		puts [string trim $::buf]
		catch "uplevel #0 {$::buf}"
		set ::buf {}}}

proc go {} {
    if {[eof stdin]} exit
    set line [gets stdin]
    interpret $line }

fileevent stdin readable go
entloop
