# An initial hackish attempt at implementing ents as described in README.
#
# To run the test code do:
#
#     ; cat ents.tcl - | ./o.sdlsh 

package require snit 2

proc t args { puts [list "trace:" {*}$args] }
set ents [list]
proc entapply {} {
	white
	set ::ents [lsearch -all -inline -not -exact $::ents HACK]
	foreach ent $::ents { $ent apply }
	flip }

snit::type ent {
	variable cmd
	constructor val { set cmd $val; lappend ::ents $self }
	destructor { lset ::ents [lsearch -exact $::ents $self] HACK }
	method GetSet {i n argz} {
		switch [llength $argz] {
			0 { lindex $cmd $i }
			1 { lset cmd $i [lindex $argz 0] }
			default { throw "Too many arguements to '$self $n'" }}}
	method apply {} { eval $cmd }
	method radius args { $self GetSet 1 radius $args }
	method pos args { $self GetSet 2 pos $args }
	method color args { $self GetSet 3 color $args }}

every 10 entapply
eval {
	ent c1 {circle 10 {0 0} {128 32 64}}
	ent c2 {circle 100 {100 100} {0 255 0}}
	ent c3 {circle 40 {300 200} {0 0 255}}}

proc Explode {e r} {
	if {$r <= 0} { $e destroy; return }
	$e radius $r
	incr r -1
	after 10 [list Explode $e $r] }

set shot [sound shot.wav]
proc explode {r x y} {
	play $::shot
	set e [ent %AUTO% [list circle $r [list $x $y] {255 0 0}]]
	Explode $e $r }

proc shift {ent offset} {
	set p [$ent pos]
	set p0 [lindex $p 0]
	set p1 [lindex $p 1]
	set p0 [expr ( $p0 + $offset ) % 800]
	set p1 [expr ( $p1 + $offset ) % 600]
	$ent pos [list $p0 $p1] }

proc random i { expr int ( rand ( ) * $i ) }

proc every_flake {ms command} {
	uplevel #0 $command
	after [random [expr 2 * $ms]] [list every_flake $ms $command] }

every_flake 500 { explode 40 [random 800] [random 600] }

ent cursor {circle 50 {-100 -100} {0 0 0}}
set keys {}
proc oninput d {
	set keys [dict get $d keys]
	lassign [dict get $d mouse] x y
	if {[llength $keys] > [llength $::keys]} { explode 40 $x $y }
	set ::keys $keys
	cursor pos [list $x $y] }

every 31 { shift ::c1 10 }
every 700 { shift ::c2 30 }
