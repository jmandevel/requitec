all: tokens parsed situated symbols ir assembly object

tokens:
	../build/requitec --emit=tokens ../build/test.rq -o ../build/test.tokens.csv --no-comment

parsed:
	../build/requitec --emit=parsed ../build/test.rq -o ../build/test.parsed.srq --no-comment

situated:
	../build/requitec --emit=situated ../build/test.rq -o ../build/test.situated.srq --no-comment

symbols:
	../build/requitec --emit=symbols ../build/test.rq -o ../build/test.symbols.json --no-comment

ir:
	../build/requitec --emit=ir ../build/test.rq -o ../build/test.ir --no-comment

assembly:
	../build/requitec --emit=assembly ../build/test.rq -o ../build/test.s --no-comment

object:
	../build/requitec --emit=object ../build/test.rq -o ../build/test.o --no-comment