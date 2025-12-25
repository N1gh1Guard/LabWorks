# ===== toolchain =====
CXX      := g++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -Wpedantic -Ihpp -Itest

# ===== layout =====
SRC_DIR   := cpp
TEST_DIR  := test
OBJ_DIR   := build/obj
BIN_DIR   := build/bin

APP_TARGET  := $(BIN_DIR)/laba2_demo
TEST_TARGET := $(BIN_DIR)/laba2_tests

SRC_FILES    := $(wildcard $(SRC_DIR)/*.cpp) main.cpp
LIB_SOURCES  := $(filter-out main.cpp,$(SRC_FILES))
TEST_FILES   := $(wildcard $(TEST_DIR)/*.cpp)

# Создаем объектные файлы с сохранением структуры директорий
APP_OBJECTS  := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LIB_OBJECTS  := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(LIB_SOURCES))
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/$(TEST_DIR)/%.o,$(TEST_FILES))

.PHONY: all clean test test-s run dirs rebuild qt qt-build qt-run console-run

all: dirs $(APP_TARGET)

dirs:
	@mkdir -p $(OBJ_DIR)/$(SRC_DIR) $(OBJ_DIR)/$(TEST_DIR) $(BIN_DIR)

$(APP_TARGET): $(APP_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Правило для main.cpp в корне
$(OBJ_DIR)/main.o: main.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для файлов в cpp/
$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: dirs $(TEST_TARGET)
	$(TEST_TARGET) $(TEST_FLAGS)

test-s: dirs $(TEST_TARGET)
	$(TEST_TARGET) -s

$(TEST_TARGET): $(TEST_OBJECTS) $(LIB_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

qt-build:
	@cd qt && mkdir -p build && cd build && (cmake .. > /dev/null 2>&1 && make > /dev/null 2>&1 || (cmake .. && make))

qt-run: qt-build
	@cd qt/build && ./Laba2GUI

console-run: all
	$(APP_TARGET)

run: qt-run

qt:
	@echo "Use the following command in Qt Creator custom steps:"
	@echo "   make -C $(CURDIR) all"

clean:
	@rm -rf build
	@rm -rf qt/build

rebuild: clean all