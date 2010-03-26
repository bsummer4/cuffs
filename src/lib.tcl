if {[info exists libloaded]} return
set libloaded 1

proc this {x} { return $x }
proc with_result {code} {
    uplevel {set result {}}
    uplevel $code
    uplevel {this $result}}

# Arithmetic
set operators [list + - * / < > <= >= == !=]
foreach operator $operators {
    proc $operator {x y} "expr \$x $operator \$y" }
proc zero? {x} {== $x 0}
proc random {max} {expr int(rand() * $max)}
proc random_from_range {min max} {+ [random [- $max $min]] $min}
proc random_choice {list} {lindex $list [random [llength $list]]}
proc dict_random_choice {dict} { random_choice [dict value $dict] }
proc dict_pairs {dict} {
    set temp {}
    with_result {
        set ii 0
        foreach item $dict {
            incr ii
            lappend temp $item
            if {$ii == 2} { lappend result $temp; set ii 0; set temp {}}}}}

# Logic
proc not {x} {== $x 0}
proc or {forms} {
    foreach form $forms { set t [uplevel $form] if $t { return $t }}
    return 0 }
proc and {forms} {
    foreach form $forms { if [not [uplevel $form]] { return 0 }}
    return 1 }

# Misc
proc cons {item list} { concat [list $item] $list }
proc identity {x} { return $x }
proc this {x} { identity $x }
proc equal {x y} {string equal $x $y}
proc empty? {string} {zero? [string length $string]}
proc lempty? {list} {zero? [llength $list]}
proc lexists? {list item} {+ 1 [lsearch -exact $list $item]}

proc shellproc {command {procname {}}} {
    if {[string equal {} $procname]} { set procname $command }
    proc $procname args " eval \[concat exec $command \$args\] "}

# proc partial {fn args} {
    # set initial $args
    # return [list args [list apply* $fn "\[concat $initial \$args\]"]] }

proc puts* {args} { puts $args }

proc lwithout {list item} {
    loop {
        set index [lsearch $list $item]
        if {$index == -1} { return $list }
        set list [lreplace $list $index $index] }}

proc eval_file {filename} {
    set file [open $filename r]
    while 1 { set line [gets $file]
        if [eof $file] break
        if [not [empty? $line]] { eval $line }}
    close $file }

proc do {shell_string} {
    # Like exec but let's sh handle everything
    catch {exec /bin/sh -c $shell_string} results options
    return $results }

proc range {bound} {
    with_result {
        for {set i 0} {$i < $bound} {incr i} {
            lappend result $i }}}

proc map {fn list} {
    # map {x {+ 1 $x}} { 3  4  5 } -> {4 5 6}
    with_result {
        foreach item $list { lappend result [apply $fn $item] }}}

proc filter {command-template list} {
    # filter {even? $1} { 3  4  5 } -> {4}
    set result [list]
    foreach 1 $list {
        set value [eval ${command-template}]
        if $value {lappend result $1} }
    return $result }

proc noop {} { return 0 }


# Macros
proc set* {varlist value} {
    foreach var $varlist {
        uplevel "set $var $value" }}

proc choice code {
    # Randomly run one of the statements in the body of $code
    uplevel [random_choice [split [string trim $code] ";\n"]] }

# TODO setf
# set group {0 1 2 3 4 {5 6 7 8}}
# setf {list group 3} a
# setf {dict {dict group 4} 5} b
# setf {dict {list group 5} 7} c
# puts $group
# -> 0 1 2 a 4 {5 b 7 c}

# TODO make it work like this
# lbind {a b c} {3 4 5}
# bind {list d {dict key1 e}} {0 {key1 3 key2 4}}
# puts [list $a $b $c $d $e]
# -> 3 4 5 0 3
proc bind {form values} {
    foreach ii [range [llength $form]] {
        uplevel [list set [lindex $form $ii] [lindex $values $ii]] }}

proc lbind {variables values} {
    foreach ii [range [llength $variables]] {
        uplevel [list set [lindex $variables $ii] [lindex $values $ii]] }}

proc dolines {filename code} {
    set f [open $filename r]
    for {set linenum 0} {[gets $f line] != -1} {incr linenum} {
        uplevel "set line {$line}; set linenum {$linenum}; $code"}
    close $f }
proc loop {code} { uplevel [list while {1} $code] }

# Redefinitions
rename apply apply-lambda
proc apply {fn args} { apply* $fn $args }
proc apply* {fn arguments} {
    if {[llength $fn] == 1} { eval [cons $fn $arguments] } \
    else { eval [cons apply-lambda [cons $fn $arguments]] }}
