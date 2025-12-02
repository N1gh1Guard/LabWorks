CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I.
PROGRAM = program
TEST = tests

SOURCES = main.cpp UI.cpp Exception.cpp
OBJECTS = $(SOURCES:.cpp=.o)

TEST_SOURCES = tests.cpp Exception.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

HEADERS = Exception.h Sequence.h DynamicArray.h LinkedList.h \
          ArraySequence.h ListSequence.h \
          ImmutableArraySequence.h ImmutableListSequence.h \
          Functions.h Pair.h UI.h

all: $(PROGRAM) $(TEST)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(PROGRAM)

$(TEST): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(TEST)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(PROGRAM)
	./$(PROGRAM)

test: $(TEST)
	./$(TEST)

clean:
	rm -f *.o $(PROGRAM) $(TEST)

.PHONY: all run test clean