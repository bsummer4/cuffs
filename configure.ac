AC_INIT([Fistacuffs],[1.0beta],[])
AC_PROG_CXXCPP
AC_PROG_CXX
AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE([foreign 1.9])
AC_LANG([C++])
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])
AC_TYPE_SIZE_T
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])

# libboost
AC_CHECK_HEADER(boost/thread.hpp, FOUND_BOOSTTHREAD=1, FOUND_BOOSTTHREAD=0)
if test $FOUND_BOOSTTHREAD != 1; then
    AC_MSG_ERROR([boost/thread.hpp Not found but is needed.])
fi

PKG_CHECK_MODULES(SDL, sdl >= 1.2)

# libsdl
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
    AC_MSG_ERROR([SDL_gfx Not Not found but is needed.])
fi

# Doxygen
# We only want HTML output, defaults are PDF, PS, and HTML.
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([$PACKAGE_NAME],[doxygen.cfg],[doxygen])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 object/Makefile
                 data/Makefile
                 object/test/Makefile
                 utils/Makefile])
AC_OUTPUT
