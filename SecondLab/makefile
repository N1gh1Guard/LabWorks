CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

all: tests lab2
	@clear
	@./tests
	@./lab2

UI: lab2
	@clear
	@./lab2

tests: tests.o Error.o
	@clear
	@$(CXX) $(CXXFLAGS) tests.o Error.o -o tests
	@./tests

lab2: main.o UI.o Error.o
	@$(CXX) $(CXXFLAGS) main.o UI.o Error.o -o lab2

main.o: main.cpp UI.h
	@$(CXX) $(CXXFLAGS) -c main.cpp

UI.o: UI.cpp UI.h ListSequence.h Error.h
	@$(CXX) $(CXXFLAGS) -c UI.cpp

tests.o: tests.cpp DynamicArray.h LinkedList.h ArraySequence.h ListSequence.h Sequence.h Error.h
	@$(CXX) $(CXXFLAGS) -c tests.cpp

Error.o: Error.cpp Error.h Training.h
	@$(CXX) $(CXXFLAGS) -c Error.cpp

clean:
	@rm -f *.o lab2 tests