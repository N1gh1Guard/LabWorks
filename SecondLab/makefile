
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

MAIN_SRCS = main.cpp UI.cpp Error.cpp
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
MAIN_TARGET = lab

TEST_SRCS = tests.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_TARGET = tests

.PHONY: all clean run test


all: $(MAIN_TARGET) $(TEST_TARGET)

$(MAIN_TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAIN_TARGET) $(MAIN_OBJS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@del /f /q *.o lab.exe tests.exe *.d

run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)