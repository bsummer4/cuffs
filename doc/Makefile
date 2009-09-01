all: readme.html design.html
readme.html: readme.rst; rst2html $^ > $@
design.html: design.rst; rst2html $^ > $@
clean:; rm readme.html design.html
