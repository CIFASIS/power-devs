all: parser.y scanner.l 
	bison -d --debug -hparser.h -o parser.cpp parser.y
	flex -d -oscanner.cpp scanner.l 
	g++ -I. -I../data -g main.cpp scanner.cpp parser.cpp -DTEST
	
