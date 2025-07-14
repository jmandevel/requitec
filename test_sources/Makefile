all: tokens parsed situated implemented symbols ir assembly object

tokens:
	./requite --emit=tokens test.rq -o test.tokens.csv

parsed:
	./requite --emit=parsed test.rq -o test.parsed.rq

situated:
	./requite --emit=situated test.rq -o test.situated.rq

symbols:
	./requite --emit=symbols test.rq -o test.symbols.trip

implemented:
	./requite --emit=implemented test.rq -o test.implemented.rq

ir:
	./requite --emit=ir test.rq -o test.ir

assembly:
	./requite --emit=assembly test.rq -o test.s

object:
	./requite --emit=object test.rq -o test.o