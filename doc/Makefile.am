doc_DATA = doc.html

# TODO Doxygen stuff should be built with the other things in this
# directory

clean-local:; rm -f doc.rst doc.html

doc.rst: readme.rst changes.rst design.rst news.rst
	cat $^ > $@

%.html: %.rst ; rst2html $< > $@
%.latex: %.rst ; rst2latex $< > $@
%.pdf: %.latex ; pdflatex -interaction=batchmode $<
