# initial information about the project
AC_INIT([Fistacuffs],[0.1],[])

# check if the source folder is available
#AC_CONFIG_SRCDIR([src/MYSOURCEFILE.cpp])

# check for a c compiler
AC_PROG_CC_C99

# check for C++ preprocessor and compiler
AC_PROG_CXXCPP
AC_PROG_CXX

# Libtool for building libraries
AC_PROG_LIBTOOL

# automake initialization including a check for automake API version >= 1.9
AM_INIT_AUTOMAKE([1.9])

# use the C++ compiler for the following checks
AC_LANG([C++])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# distribute additional compiler and linker flags among Makefiles
# --> set these variables instead of CXXFLAGS or LDFLAGS (for user only)
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

########################################################################
# DOXYGEN SUPPORT
# ######################################################################

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN([$PACKAGE_NAME],[doxygen.cfg],[doc/doxygen])

# ######################################################################

# files to generate via autotools (prepare .am or .in source files)
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/switchbox/Makefile
                 tests/Makefile
                 tests/switchbox/Makefile
                 doc/Makefile])

# finally this generates the Makefiles etc. for the build
AC_OUTPUT
