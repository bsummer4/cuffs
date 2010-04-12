proc list_with list {
    for {set i 0} {$i < [llength $list]} {incr i} {
        uplevel "set $i [lindex $list $i]" }}

proc append args {
    set result [list]
    foreach arg $args {
        foreach elt $arg {
            lappend result $elt }}
    return $result }

load ./sdl.so

proc circle {radius pos color} {
    eval [append draw_circle $radius $color $pos] }
