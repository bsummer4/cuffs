package require XOTcl
namespace import xotcl::*

load ./bitfield.so
Class create Bitfield

proc this x { return $x }

Bitfield instproc init {width height} {
    my set bitfield [bitfield_new $width $height]
    my set height $height
    my set width $width }

Bitfield instproc destroy {} {
    bitfield_delete [my set bitfield]
    [my info class] instdestroy [self] }

Bitfield instproc pset {x y args} {
    my instvar bitfield
    switch [llength $args] {
        0 { bitfield_get $bitfield $x $y }
        1 { bitfield_set $bitfield $x $y $args
            this $args }
        default { error {pset only accepts 3 arguments} }}}
