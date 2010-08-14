package require snit 2 

proc random i { expr int ( rand ( ) * $i ) }
proc t args { puts stderr [list "trace:" {*}$args] }
proc list_with list {
	for {set i 0} {$i < [llength $list]} {incr i} {
		uplevel "set $i [lindex $list $i]" }}

proc appendl args {
	set result [list]
	foreach arg $args {
		foreach elt $arg {
		lappend result $elt }}
	return $result }

proc every {ms command} {
	uplevel #0 $command
	after $ms [list every $ms $command] }


# Slave Interpeter

interp create -safe safe
foreach proc {Circle Rect Arrow} { safe alias $proc $proc }
proc safeeval str { safe eval [list eval $str] }


# Ents

set ents [list]
proc entapply {} {
	white
	set ::ents [lsearch -all -inline -not -exact $::ents HACK]
	foreach ent $::ents { $ent apply }
	flip }

# This maintains a command specifying how to draw something, and manages a
# reference to itself in ::ents.
snit::type ent {
	variable cmd
	constructor val { set cmd $val; lappend ::ents $self }
	destructor { lset ::ents [lsearch -exact $::ents $self] HACK }
	method apply {} { eval $cmd }
	method get i { lindex $cmd $i }
	method set {i args} { lset cmd $i $args }}

# The general protocol for ents is:
#
# - pos -> {x y}
# - pos {x y} -> _
# - color -> {r g b a}
# - color {r g b a} -> _

snit::type Circle {
	variable ent
	constructor {r center color} {
		set ent [ent $self.ent [list circle $r $center $color]]
		safe alias $self $self }
	destructor { $ent destroy }
	method radius {{r _}} {
		if {[string equal $r _]} { return [$ent get 1] }
		$ent set 1 $r }
	method pos {{x _} {y _}} {
		if {[string equal $x _]} { return [$ent get 2] }
		$ent set 2 $x $y }
	method color {{r _} {g _} {b _} {a _}} {
		if {[string equal $r _]} { return [$ent get 3] }
		$ent set 3 $r $g $b $a }}

snit::type Rect {
	variable ent

	destructor { $ent destroy }
	constructor {p1 p2 color} {
		set ent [::ent $self.ent [list rect $p1 $p2 $color]]
		safe alias $self $self }

	method point1 {{x _} {y _}} {
		if {[string equal $x _]} { return [$ent get 1] }
		$ent set 1 $x $y }
	method point2 {{x _} {y _}} {
		if {[string equal $x _]} { return [$ent get 2] }
		$ent set 2 $x $y }
	method color {{r _} {g _} {b _} {a _}} {
		if {[string equal $r _]} { return [$ent get 3] }
		$ent set 3 $r $g $b $a }
	method pos {{x _} {y _}} {
		if {[string equal $x _]} { return [$self center] }
		lassign [$self center] oldx oldy
		$self shift [expr $x - $oldx] [expr $y - $oldy] }

	method height {} {
		lassign [$self point1] _ top
		lassign [$self point2] _ bottom
		expr $bottom - $top }
	method width {} {
		lassign [$self point1] left _
		lassign [$self point2] right _
		expr $right - $left }
	method center {} {
		lassign [$self point1] left top
		set xadd [expr [$self width] / 2]
		set yadd [expr [$self height] / 2]
		list [expr $left + $xadd] [expr $top + $yadd] }
	method shift {x y} {
		lassign [$self point1] left top
		lassign [$self point2] right bottom
		incr left $x; incr right $x
		incr top $y; incr bottom $y
		$self point1 $left $top
		$self point2 $right $bottom }}

snit::type Arrow {
	variable ent

	destructor { $ent destroy }
	constructor {p1 p2 color} {
		set ent [::ent $self.ent [list arrow $p1 $p2 $color]]
		safe alias $self $self }

	method base {{x _} {y _}} {
		if {[string equal $x _]} { return [$ent get 1] }
		$ent set 1 $x $y }
	method tip {{x _} {y _}} {
		if {[string equal $x _]} { return [$ent get 2] }
		$ent set 2 $x $y }
	method color {{r _} {g _} {b _} {a _}} {
		if {[string equal $r _]} { return [$ent get 3] }
		$ent set 3 $r $g $b $a }
	method pos {{x _} {y _}} { $self base $x $y }}


# Input handling

set buf {}
proc interpret line {
	append ::buf $line "\n"
	if {[info complete $::buf]} {
		if {[catch [list safeeval $::buf] msg]} {
			puts stderr "Error: $msg" }
		set ::buf {}}}

proc go {} {
	if {[eof stdin]} exit
	set line [gets stdin]
	interpret $line }

proc oninput dict { puts [list input $dict] }

fileevent stdin readable go
