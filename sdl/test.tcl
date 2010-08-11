# An initial hackish attempt at implementing ents as described in README.
#
# To run the test code do:
#
#     ; ./o.sdlsh ents.tcl

set granularity 20 ;# 50 fps
sound boom shot.wav
image bg map.pgm

ent .bg {bg 0 0}
circle_ent .c1 10 {0 0} {128 32 64 200}
circle_ent .c2 100 {100 100} {0 255 0 200}
circle_ent .c3 40 {300 200} {0 0 255 200}

set rd [expr $granularity / 10.0 ]
proc Explode {e r} {
	set ri [expr int ( $r )]
	if {$ri <= 0} { $e destroy; return }
	$e radius $ri
	set r [expr $r - $::rd]
	after $::granularity [list Explode $e $r] }

proc explode {r x y} {
	boom
	draw-on bg
	circle $r [list $x $y] {255 255 255 255}
	draw-on
	set e [circle_ent %AUTO% $r [list $x $y] {255 0 0 200}]
	Explode $e $r }

proc shift {ent x y} {
	lassign [$ent pos] p0 p1
	set p0 [expr ( $p0 + $x ) % 800]
	set p1 [expr ( $p1 + $y ) % 600]
	$ent pos [list $p0 $p1] }

proc every_flakey {ms command} {
	uplevel #0 $command
	after [random [expr 2 * $ms]] [list every_flakey $ms $command] }

circle_ent .cursor 50 {0 0} {0 0 0 200}
set keys {}
proc oninput d {
	set keys [dict get $d keys]
	lassign [dict get $d mouse] x y
	dict for {key offset} {left {-10 0}
	                       right {10 0}
	                       up {0 -10}
	                       down {0 10}} {
		if {-1 == [lsearch $keys $key]} continue
		set keys [lsearch -all -inline -not -exact $keys $key]
		shift .c3 {*}$offset
		puts $key  }
	if {[llength $keys] > [llength $::keys]} {
		puts shoot
		explode 40 $x $y }
	set ::keys $keys
	.cursor pos [list $x $y] }

every_flakey 500 { explode 40 [random 800] [random 600] }
every 6 { shift .c1 2 2 }
every 70 { shift .c2 3 -1 }
every $granularity entapply
entloop
