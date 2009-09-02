doc_DATA = readme.html design.html

# TODO Doxygen stuff should be built with the other things in this
# directory

%.html: %.rst ; rst2html $< > $@
%.latex: %.rst ; rst2latex $< > $@
%.pdf: %.latex ; pdflatex -interaction=batchmode $<
