# DOXYGEN SUPPORT
include $(top_srcdir)/aminclude.am
ACLOCAL_AMFLAGS = -I m4

# ensure the distribution of the doxygen configuration file
EXTRA_DIST = doxygen.cfg

SUBDIRS=src tests gamefiles

C_STYLE = -npsl -npcs -bli0 -brs -brf -br -nut
CPP_STYLE = -Uas2

fixstyle: clean
	echo $(CSTYLE) `find . -name '*.[ch]'`
	echo $(CPPSTYLE) `find . -name '*.[ch]pp'`
	indent $(CSTYLE) `find . -name '*.[ch]'`
	astyle $(CPPSTYLE) `find . -name '*.[ch]pp'`
