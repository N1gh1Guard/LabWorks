# Makefile for Pathfinding Map Application
# C++17 with Qt5

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
MOC = moc

# Qt5 paths (adjust if Qt5 is installed in a non-standard location)
QT5_INCLUDE = $(shell pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets 2>/dev/null)
QT5_LIBS = $(shell pkg-config --libs Qt5Core Qt5Gui Qt5Widgets 2>/dev/null)

# If pkg-config doesn't work, try qmake
ifeq ($(QT5_INCLUDE),)
	QT5_INCLUDE = -I/usr/include/qt -I/usr/include/qt/QtCore -I/usr/include/qt/QtGui -I/usr/include/qt/QtWidgets
	QT5_LIBS = -lQt5Core -lQt5Gui -lQt5Widgets
endif

# Directories
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
MOC_DIR = $(BUILD_DIR)/moc

# Source files
CORE_SRCS = $(SRC_DIR)/core/Cell.cpp \
            $(SRC_DIR)/core/Exceptions.cpp \
            $(SRC_DIR)/core/Graph.cpp \
            $(SRC_DIR)/core/Map.cpp

ALGORITHMS_SRCS = $(SRC_DIR)/algorithms/Pathfinder.cpp

GENERATION_SRCS = $(SRC_DIR)/generation/MapGenerator.cpp

IO_SRCS = $(SRC_DIR)/io/MapIO.cpp

GUI_SRCS = $(SRC_DIR)/gui/main.cpp \
           $(SRC_DIR)/gui/mainwindow.cpp \
           $(SRC_DIR)/gui/mapwidget.cpp

TEST_SRCS = $(SRC_DIR)/tests/tests.cpp

# Qt classes that need MOC
MOC_HEADERS = $(SRC_DIR)/gui/mainwindow.hpp \
              $(SRC_DIR)/gui/mapwidget.hpp

MOC_SRCS = $(MOC_DIR)/mainwindow.moc.cpp \
           $(MOC_DIR)/mapwidget.moc.cpp

# Object files
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
ALGORITHMS_OBJS = $(ALGORITHMS_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
GENERATION_OBJS = $(GENERATION_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
IO_OBJS = $(IO_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
GUI_OBJS = $(GUI_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
MOC_OBJS = $(OBJ_DIR)/mainwindow.moc.o \
           $(OBJ_DIR)/mapwidget.moc.o
TEST_OBJS = $(TEST_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

ALL_OBJS = $(CORE_OBJS) $(ALGORITHMS_OBJS) $(GENERATION_OBJS) $(IO_OBJS) $(GUI_OBJS) $(MOC_OBJS)
TEST_ALL_OBJS = $(CORE_OBJS) $(ALGORITHMS_OBJS) $(GENERATION_OBJS) $(IO_OBJS) $(TEST_OBJS)

# Include directories
INCLUDES = -I$(SRC_DIR) $(QT5_INCLUDE)

# Targets
MAIN_TARGET = $(BUILD_DIR)/pathfinding_app
TEST_TARGET = $(BUILD_DIR)/tests

.PHONY: all clean test app run

# Default target: build and run
all: run

# Main application (build only)
app: $(MAIN_TARGET)

# Build and run the application
run: app
	./$(MAIN_TARGET)

# Test executable
test: $(TEST_TARGET)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/core $(OBJ_DIR)/algorithms $(OBJ_DIR)/generation $(OBJ_DIR)/io $(OBJ_DIR)/gui $(OBJ_DIR)/tests

$(MOC_DIR):
	mkdir -p $(MOC_DIR)

# MOC rules for Qt classes
$(MOC_DIR)/mainwindow.moc.cpp: $(SRC_DIR)/gui/mainwindow.hpp | $(MOC_DIR)
	$(MOC) $(INCLUDES) $< -o $@

$(MOC_DIR)/mapwidget.moc.cpp: $(SRC_DIR)/gui/mapwidget.hpp | $(MOC_DIR)
	$(MOC) $(INCLUDES) $< -o $@

# Compile MOC-generated files
$(OBJ_DIR)/mainwindow.moc.o: $(MOC_DIR)/mainwindow.moc.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/mapwidget.moc.o: $(MOC_DIR)/mapwidget.moc.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile source files
$(OBJ_DIR)/core/%.o: $(SRC_DIR)/core/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/algorithms/%.o: $(SRC_DIR)/algorithms/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/generation/%.o: $(SRC_DIR)/generation/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/io/%.o: $(SRC_DIR)/io/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/gui/%.o: $(SRC_DIR)/gui/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/tests/%.o: $(SRC_DIR)/tests/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Link main application
$(MAIN_TARGET): $(ALL_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(ALL_OBJS) $(QT5_LIBS)

# Link test executable
$(TEST_TARGET): $(TEST_ALL_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_ALL_OBJS)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build and run the main application (default)"
	@echo "  app     - Build the main application only"
	@echo "  run     - Build (if needed) and run the application"
	@echo "  test    - Build the test executable"
	@echo "  clean   - Remove all build artifacts"
	@echo "  help    - Show this help message"
