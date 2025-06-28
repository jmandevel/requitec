all: tokens parsed situated contextualized symbols ir assembly object

tokens:
	./requite --emit=tokens test.rq -o test.tokens.csv

parsed:
	./requite --emit=parsed test.rq -o test.parsed.rq

situated:
	./requite --emit=situated test.rq -o test.situated.rq

contextualized:
	./requite --emit=contextualized test.rq -o test.contextualized.rq

symbols:
	./requite --emit=symbols test.rq -o test.symbols.trip

ir:
	./requite --emit=ir test.rq -o test.ir

assembly:
	./requite --emit=assembly test.rq -o test.s

object:
	./requite --emit=object test.rq -o test.o