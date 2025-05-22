CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

all: lab2 tests

lab2: main.o UI.o Error.o
	@$(CXX) $(CXXFLAGS) main.o UI.o Error.o -o lab2

tests: tests.o UI.o Error.o
	@$(CXX) $(CXXFLAGS) tests.o UI.o Error.o -o tests

main.o: main.cpp UI.h
	@$(CXX) $(CXXFLAGS) -c main.cpp

UI.o: UI.cpp UI.h
	@$(CXX) $(CXXFLAGS) -c UI.cpp

tests.o: tests.cpp UI.h
	@$(CXX) $(CXXFLAGS) -c tests.cpp

Error.o: Error.cpp Error.h
	@$(CXX) $(CXXFLAGS) -c Error.cpp

clean:
	@cls && del *.o && del lab2 && del tests