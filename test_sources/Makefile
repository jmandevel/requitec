all: tokens parsed situated ir assembly object

tokens:
	../build/requitec --emit=tokens test.rq -o test.tokens.csv

parsed:
	../build/requitec --emit=parsed test.rq -o test.parsed.srq

situated:
	../build/requitec --emit=situated test.rq -o test.situated.srq

ir:
	../build/requitec --emit=ir test.rq -o test.ir

assembly:
	../build/requitec --emit=assembly test.rq -o test.s

object:
	../build/requitec --emit=object test.rq -o test.o