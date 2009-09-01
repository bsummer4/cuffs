all: readme.html design.html
clean:; rm -rf *.html *.latex *.pdf *.out *.log *.aux html latex

%.html: %.rst ; rst2html $< > $@
%.latex: %.rst ; rst2latex $< > $@
%.pdf: %.latex ; pdflatex -interaction=batchmode $<

html: doxygen.cfg; doxygen $^
latex: doxygen.cfg; doxygen $^
