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

set granularity 15

every $granularity entapply
eval {
	ent c1 {circle 10 {0 0} {128 32 64 200}}
	ent c2 {circle 100 {100 100} {0 255 0 200}}
	ent c3 {circle 40 {300 200} {0 0 255 200}}}

set rd [expr $granularity / 10.0 ]
proc Explode {e r} {
	set ri [expr int ( $r )]
	if {$ri <= 0} { $e destroy; return }
	$e radius $ri
	set r [expr $r - $::rd]
	after $::granularity [list Explode $e $r] }

sound .boom shot.wav
proc explode {r x y} {
	.boom
	set e [ent %AUTO% [list circle $r [list $x $y] {255 0 0 200}]]
	Explode $e $r }

proc shift {ent x y} {
	lassign [$ent pos] p0 p1
	set p0 [expr ( $p0 + $x ) % 800]
	set p1 [expr ( $p1 + $y ) % 600]
	$ent pos [list $p0 $p1] }

proc random i { expr int ( rand ( ) * $i ) }

proc every_flakey {ms command} {
	uplevel #0 $command
	after [random [expr 2 * $ms]] [list every_flakey $ms $command] }

every_flakey 500 { explode [random 80] [random 800] [random 600] }

ent cursor {circle 50 {0 0} {0 0 0 200}}
set keys {}
proc oninput d {
	set keys [dict get $d keys]
	lassign [dict get $d mouse] x y
	if {[llength $keys] > [llength $::keys]} { explode 40 $x $y }
	set ::keys $keys
	cursor pos [list $x $y] }

every 6 { shift ::c1 2 2 }
every 70 { shift ::c2 3 -1 }
