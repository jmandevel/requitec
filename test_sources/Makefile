all: tokens parsed situated ir assembly object

tokens:
	./requite --emit=tokens test.rq -o test.tokens.csv

parsed:
	./requite --emit=parsed test.rq -o test.parsed.srq

situated:
	./requite --emit=situated test.rq -o test.situated.srq

ir:
	./requite --emit=ir test.rq -o test.ir

assembly:
	./requite --emit=assembly test.rq -o test.s

object:
	./requite --emit=object test.rq -o test.o