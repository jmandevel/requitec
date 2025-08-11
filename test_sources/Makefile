all: tokens parsed situated symbols implemented ir assembly object

tokens:
	./requite --emit=tokens test.rq -o test.tokens.csv

parsed:
	./requite --emit=parsed test.rq -o test.parsed.rqir

situated:
	./requite --emit=situated test.rq -o test.situated.rqir

symbols:
	./requite --emit=symbols test.rq -o test.symbols.trip

implemented:
	./requite --emit=implemented test.rq -o test.implemented.rqir

ir:
	./requite --emit=ir test.rq -o test.ir

assembly:
	./requite --emit=assembly test.rq -o test.s

object:
	./requite --emit=object test.rq -o test.o