proc list_with list {
    for {set i 0} {$i < [llength $list]} {incr i} {
        uplevel "set $i [lindex $list $i]" }}


load ./sdl.so

proc circle {radius pos color} {
    list_with $pos
    set x $0
    set y $1
    list_with $color
    draw_circle $radius $0 $1 $2 $x $y }
