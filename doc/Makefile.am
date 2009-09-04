doc_DATA = readme.html design.html doc.pdf doc.html

# TODO Doxygen stuff should be built with the other things in this
# directory

doc.rst: readme.rst authors.rst changes.rst design.rst news.rst
	cat $^ > doc.rst

%.html: %.rst ; rst2html $< > $@
%.latex: %.rst ; rst2latex $< > $@
%.pdf: %.latex ; pdflatex -interaction=batchmode $<
