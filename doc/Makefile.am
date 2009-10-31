doc_DATA = doc.html

html: readme.html changes.html design.html news.html doc.html
latex: readme.tex changes.tex design.tex news.tex 
pdf: readme.pdf changes.pdf design.pdf news.pdf

clean-local:; rm -f doc.rst doc.html

doc.rst: readme.rst changes.rst design.rst news.rst
	cat $^ > $@

%.html: %.rst ; rst2html $< > $@
%.latex: %.rst ; rst2latex $< > $@
%.pdf: %.latex ; pdflatex -interaction=batchmode $<
