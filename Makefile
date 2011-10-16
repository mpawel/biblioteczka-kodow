all: doc/kody.pdf
#	mkdir -p indent
#	indent < tekst.cpp > indent/tekst.cpp
#	mkdir -p postscript
#	enscript indent/grafy.cpp --output=postscript/grafy.ps --color=1 --highlight=cpp --style=emacs_verbose -X latin2 -b "||grafy.cpp"
#	enscript indent/tekst.cpp --output=postscript/tekst.ps --color=1 --highlight=cpp --style=emacs_verbose -X latin2 -b "||tekst.cpp"

#indent/grafy.cpp : grafy.cpp
#	indent < $< > $@
#indent/tekst.cpp : tekst.cpp
#	indent < $< > $@
#indent/geom.cpp : geom.cpp
#	indent < $< > $@

doc/kody.pdf: grafy.hpp tekst.hpp geom.hpp
	pdflatex -interaction=batchmode -output-directory=doc kody.tex

test:
	mkdir -p bin
	mkdir -p out
	g++ -O2 grafy.cpp -lm -o bin/grafy
	./bin/grafy < in/graf.in
	./bin/grafy < in/graf2.in
	./bin/grafy < in/graf3.in
	g++ -O2 tekst.cpp -lm -o bin/tekst
	./bin/tekst

clean:
	rm -rf indent/* postscript bin out doc/*
