.PHONY: test
.SILENT:

CXX=g++
CXX_FLAGS=-g -Wall -std=c++11

all:
	$(CXX) $(CXX_FLAGS) main.cpp -o main

test:
	$(CXX) $(CXX_FLAGS) test.cpp -o test
	./test

clean:
	rm -f main
	rm -rf *.dSYM