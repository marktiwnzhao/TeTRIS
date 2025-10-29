GRAMMARDIR=grammars
LAN ?= c
CFLAGS = -O3 -funroll-loops -fpermissive

all: tetris

gen-template: template/* ast.py
	@if [ ! -d "src" ]; then \
	    mkdir src; \
	    mkdir include; \
	    mkdir parser; \
	fi
	cd ${GRAMMARDIR}/${LAN} && ./replace.sh
	python2 ast.py -i ${GRAMMARDIR}/${LAN}/replaced_grammar -t ${GRAMMARDIR}/${LAN}/tokens -d data/destructor -D data/datatype -e ${GRAMMARDIR}/${LAN}/extra_flex_rule_${LAN} -s ${GRAMMARDIR}/${LAN}/semantic.json
	cd parser && flex flex.l && bison bison.y --output=bison_parser.cpp --defines=bison_parser.h --verbose -Wcounterexamples -Wconflicts-rr

tetris: gen-template
	cd ../core && make

clean:
	rm -f include/* parser/* src/*
	rm -f grammar/*/replaced_grammar grammar/*/tokens
	rm -f *.pyc
FORCE:
