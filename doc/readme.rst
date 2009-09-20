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

