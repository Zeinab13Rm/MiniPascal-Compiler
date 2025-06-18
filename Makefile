all:
	bison -dtv -o parser.cpp --defines=parser.h parser.y  
	flex -oscanner.cpp ./lexer.l   
	g++ -std=c++17 -o minipascal ./program.cpp ./lexer.cpp ./parser.cpp  ./ast.cpp ./semantic_analyzer.cpp ./codegenerator.cpp 
