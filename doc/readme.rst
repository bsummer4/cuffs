==========
Fistacuffs
==========

Awesome

Setup
=====

Layout
------

- /doc -- All documentation goes here.  Formatted in `ReStructured Text`_
- /Bootstrap -- Shell script for generating the build.
- /src -- source code
- /src/switchbox -- source code for the networking code and the switchbox.
- /tests -- Testing programs to test code in source.
- /tests/switchbox -- testing programs for the switchbox



Prerequisites
-------------

None so far


Building
--------
::

 $ ./Bootstrap
 $ ./configure
 $ make
 $ make install


Building Documentation
----------------------
:: 

 $ make doxygen-doc

Builds into ./doc/doxygen

.. _ReStructured Text: http://en.wikipedia.org/wiki/ReStructuredText


Contributing
============

Coding Style
============

- C source files are .c and .h, c++ source files are .cpp and .hpp

- Run Code beautifiers on code before you commit.

  - For c++::
    astyle -Udbs2 < *.cpp *.hpp

  - for c::
    indent -bli0 *.c *.h
