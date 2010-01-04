# initial information about the project
AC_INIT([Fistacuffs],[0.3],[])

# Bootstrap told me to do it!  TODO This comment is not clear
AC_CONFIG_MACRO_DIR([m4])

# Compilers and build tools
AC_PROG_CXXCPP
AC_PROG_CXX
AC_PROG_LIBTOOL

# Automake initialization including a check for automake API version >= 1.9
AM_INIT_AUTOMAKE([foreign 1.9])

# Use the C++ compiler for the following checks
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

# Check for Libraries
## boost
AC_CHECK_HEADER(boost/thread.hpp, FOUND_BOOSTTHREAD=1, FOUND_BOOSTTHREAD=0)
if test $FOUND_BOOSTTHREAD != 1; then
    AC_MSG_ERROR([boost/thread.hpp Not found. Please make sure you have it installed.])
fi

PKG_CHECK_MODULES(SDL, sdl >= 1.2)

## libsdl
AC_CHECK_HEADER(SDL/SDL.h, FOUND_SDL=1, FOUND_SDL=0)
AC_CHECK_HEADER(SDL/SDL_image.h, FOUND_SDL_IMAGE=1, FOUND_SDL_IMAGE=0)
AC_CHECK_HEADER(SDL/SDL_gfxPrimitives.h, FOUND_SDL_GFX=1, FOUND_SDL_GFX=0)
if test $FOUND_SDL != 1; then
    AC_MSG_ERROR([SDL.h Not found but is needed.])
fi
if test $FOUND_SDL_IMAGE != 1; then
    AC_MSG_ERROR([SDL_image.h Not found but is needed.])
fi
if test $FOUND_SDL_IMAGE != 1; then
    AC_MSG_ERROR([SDL_mixer.h Not found but is needed.])
fi
if test $FOUND_SDL_GFX != 1; then
    AC_MSG_ERROR([SDL_gfx Not found but is needed. Please make sure it is installed.])
fi

## Doxygen
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([$PACKAGE_NAME],[doxygen.cfg],[doc/doxygen])

# Files to generate from .am or .in files
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 object/Makefile
                 object/test/Makefile
                 utils/Makefile])
AC_OUTPUT
