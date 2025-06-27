all: tokens parsed situated

tokens:
	./requite --emit=tokens test.rq -o test.csv

parsed:
	./requite --emit=parsed test.rq -o test.parsed.rq

situated:
	./requite --emit=situated test.rq -o test.situated.rq