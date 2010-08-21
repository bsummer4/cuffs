# An initial hackish attempt at implementing ents as described in README.
#
# To run the test code do:
#
#     ; ./o.sdlsh ents.tcl

set granularity 20 ;# 50 fps
set root $env(CUFFS_ROOT)
sound boom $root/data/shot.wav
image bg $root/data/map.pgm
sprite rock $root/data/rock.pgm

set meters [list]
proc wut args {}
proc mkmeter {var color} {
	set r [Rect %AUTO% {0 0} {0 0} $color]
	set id [llength $::meters]
	lappend ::meters $r
	trace add variable $var write "setmeter $id \[set $var\];wut"
	return $id }

proc setmeter {id percent} {
	set x [expr "800-10*($id+2)"]
	set y 580
	set X [expr $x + 10]
	set Y [expr "int(580 - 560*$percent)"]
	set m [lindex $::meters $id]
	$m point1 $x $y
	$m point2 $X $Y }

foreach proc {setmeter mkmeter wut} {
	safe alias $proc $proc }

snit::type Cursor {
	variable box
	variable arrow
	variable base

	# base_ is another ent from which an arrow points to the cursor
	destructor { $box destroy; $arrow destroy }
	constructor base_ {
		set base $base_
		set box [Rect $self.box {0 0} {10 10} {0 0 0 96}]
		set arrow [Arrow2 $self.arrow [$base pos] [$box pos] 15 120 {0 255 0 128}]
		safe alias $self $self }

	method update {} {
		lassign [$base pos] bx by
		lassign [$box pos] tx ty
		$arrow base $bx $by; $arrow tip $tx $ty
		set len [expr "5*int(sqrt(hypot($tx-$bx, $ty-$by)))" ]
		$arrow length $len }
	method pos {{x _} {y _}} { $box pos $x $y; $self update; $box pos }
	method color {{r _} {g _} {b _} {a _}} { $arrow color $r $g $b $a }}

mkent bg 0 0
Arrow .accel {400 50} {0 0} {0 0 255 128}
Circle .c1 10 {0 0} {128 32 64 200}
Circle .c2 100 {100 100} {0 255 0 200}
Circle .c3 15 {0 0} {0 0 255 200}
Cursor .cursor .c3

safeeval {
	proc wut args {}
	proc accelupdate {dx dy} {
		.accel tip [expr "int(400+$dx*400)"] [expr "int(50+$dy*400)"] }
	proc mkmeter_ {v c} {
		set id [mkmeter $v $c]
		trace add variable $v write "setmeter $id \[set $v\];wut"
		return $id }
	mkmeter_ ::energy {255 0 0 128}
	mkmeter_ ::health {0 0 255 128}
	trace add variable ::accel write {accelupdate {*}$::accel; wut}
	trace add variable ::pos write {
		.c3 pos {*}$::pos; .cursor update; wut}

	lassign {1 1 {.06 .07} {300 200}} energy health accel pos}

# set energy 1.00
# set health 1.00

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
	set e [Circle %AUTO% $r [list $x $y] {255 0 0 200}]
	Explode $e $r }

safe alias explode explode

proc shift {ent x y} {
	lassign [$ent pos] p0 p1
	set p0 [expr ( $p0 + $x ) % 800]
	set p1 [expr ( $p1 + $y ) % 600]
	$ent pos $p0 $p1 }

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
		puts $key }
	if {[llength $keys] > [llength $::keys]} { puts "shoot $x $y"}
	set ::keys $keys
	.cursor pos $x $y }

every $granularity entapply
entloop