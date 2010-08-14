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
foreach proc {Circle Rect Arrow t} { safe alias $proc $proc }
proc safeeval str { safe eval [list eval $str] }

# Ents
set freeents [list]
set ents [list]
proc mkent args {
	global freeents ents
	if {[llength $freeents]} {
		set i [lindex $freeents 0]
		set freeents [lrange $freeents 1 end]
		lset ents $i $args
		return $i }
	set i [llength $ents]
	lappend ents $args
	return $i }

proc killent id { lappend ::freeents $id }

proc entapply {} {
	global freeents ents
	white
	for {set i 0} {$i < [llength $ents]} {incr i} {
		if {-1 != [lsearch $freeents $i]} continue
		eval [lindex $ents $i] }
	flip }

proc entget {id index} { lindex $::ents $id $index }
proc entset {id index args} { lset ::ents $id $index $args }

# The general protocol for ents is:
#
# - pos -> {x y}
# - pos {x y} -> _
# - color -> {r g b a}
# - color {r g b a} -> _

snit::type Circle {
	variable ent
	constructor {r center color} {
		set ent [mkent circle $r $center $color]
		safe alias $self $self }
	destructor { killent $ent }
	method radius {{r _}} {
		if {[string equal $r _]} { return [entget $ent 1] }
		entset $ent 1 $r }
	method pos {{x _} {y _}} {
		if {[string equal $x _]} { return [entget $ent 2] }
		entset $ent 2 $x $y }
	method color {{r _} {g _} {b _} {a _}} {
		if {[string equal $r _]} { return [entget $ent 3] }
		entset $ent 3 $r $g $b $a }}

snit::type Rect {
	variable ent

	destructor { killent $ent }
	constructor {p1 p2 color} {
		set ent [mkent rect $p1 $p2 $color]
		safe alias $self $self }

	method point1 {{x _} {y _}} {
		if {[string equal $x _]} { return [entget $ent 1] }
		entset $ent 1 $x $y }
	method point2 {{x _} {y _}} {
		if {[string equal $x _]} { return [entget $ent 2] }
		entset $ent 2 $x $y }
	method color {{r _} {g _} {b _} {a _}} {
		if {[string equal $r _]} { return [entget $ent 3] }
		entset $ent 3 $r $g $b $a }
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

	destructor { killent $ent }
	constructor {p1 p2 color} {
		set ent [mkent arrow $p1 $p2 $color]
		safe alias $self $self }

	method base {{x _} {y _}} {
		if {[string equal $x _]} { return [entget $ent 1] }
		entset $ent 1 $x $y }
	method tip {{x _} {y _}} {
		if {[string equal $x _]} { return [entget $ent 2] }
		entset $ent 2 $x $y }
	method color {{r _} {g _} {b _} {a _}} {
		if {[string equal $r _]} { return [entget $ent 3] }
		entset $ent 3 $r $g $b $a }
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
