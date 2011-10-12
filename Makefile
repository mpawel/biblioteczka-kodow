all:
	mkdir -p indent
	indent < grafy.cpp > indent/grafy.cpp
	indent < tekst.cpp > indent/tekst.cpp
	enscript indent/grafy.cpp --output=postscript/grafy.ps --color=1 --highlight=cpp --style=emacs_verbose -X latin2 -b "||grafy.cpp"
	enscript indent/tekst.cpp --output=postscript/tekst.ps --color=1 --highlight=cpp --style=emacs_verbose -X latin2 -b "||tekst.cpp"

clean:
	rm -rf indent
	rm -rf postscript