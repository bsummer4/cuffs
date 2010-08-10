# An initial hackish attempt at implementing ents as described in README.
#
# To run the test code do:
#
#     ; cat ents.tcl - | ./o.sdlsh 

package require snit 2

proc t args { puts [list "trace:" {*}$args] }
proc HACK args {}
set ents [list]
proc entapply {} {
	white
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
	ent c1 {circle 10 {0 0} {255 0 0}}
	ent c2 {circle 100 {100 100} {0 255 0}}
	ent c3 {circle 40 {300 200} {0 0 255}}}

proc shift {ent offset} {
	set p [$ent pos]
	set p0 [lindex $p 0]
	set p1 [lindex $p 1]
	set p0 [expr ( $p0 + $offset ) % 800]
	set p1 [expr ( $p1 + $offset ) % 600]
	$ent pos [list $p0 $p1] }

every 31 { shift ::c1 10 }
every 700 { shift ::c2 30 }
after 3000 {
	foreach c {c1 c2 c3} { $c destroy }
	ent c1 {circle 15 {30 30} {200 50 5}}
	ent c2 {circle 80 {130 130} {10 255 10}}
	ent c3 {circle 45 {630 520} {45 80 255}}}
