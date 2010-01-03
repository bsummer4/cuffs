SUBDIRS=game

# Doxygen
include $(top_srcdir)/aminclude.am
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = doxygen.cfg # Doxygen configuration should be distributed

# Convenience
C_STYLE = -npsl -npcs -bli0 -brs -brf -br -nut
CPP_STYLE = -Uas2
fixstyle: clean
	indent $(C_STYLE) `find . -name '*.[ch]'`
	astyle $(CPP_STYLE) `find . -name '*.[ch]pp'`
