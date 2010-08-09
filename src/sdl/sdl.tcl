proc list_with list {
    for {set i 0} {$i < [llength $list]} {incr i} {
        uplevel "set $i [lindex $list $i]" }}

proc appendl args {
    set result [list]
    foreach arg $args {
        foreach elt $arg {
            lappend result $elt }}
    return $result }

load ./sdl.so

proc circle {radius pos color} {
    eval [appendl draw_circle $radius $color $pos] }
