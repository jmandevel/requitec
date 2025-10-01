all: tokens parsed situated ir assembly object

tokens:
	./requitec --emit=tokens test.rq -o test.tokens.csv

parsed:
	./requitec --emit=parsed test.rq -o test.parsed.srq

situated:
	./requitec --emit=situated test.rq -o test.situated.srq

ir:
	./requitec --emit=ir test.rq -o test.ir

assembly:
	./requitec --emit=assembly test.rq -o test.s

object:
	./requitec --emit=object test.rq -o test.o