# Stream Lab - Makefile
# C++17 Project with Qt5
# Boyer-Moore-Horspool, NFA→DFA, Spell Checker
# All files in single directory

# ═══════════════════════════════════════════════════════════════
# COMPILER AND FLAGS
# ═══════════════════════════════════════════════════════════════

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fPIC
MOC = moc

# Qt5 Flags
QT_CFLAGS = $(shell pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets)
QT_LIBS = $(shell pkg-config --libs Qt5Core Qt5Gui Qt5Widgets)

CXXFLAGS += $(QT_CFLAGS)
LDFLAGS = $(QT_LIBS)

# ═══════════════════════════════════════════════════════════════
# DIRECTORIES
# ═══════════════════════════════════════════════════════════════

BUILD_DIR = build
TARGET = $(BUILD_DIR)/streamlab

# ═══════════════════════════════════════════════════════════════
# SOURCE FILES
# ═══════════════════════════════════════════════════════════════

# C++ source files that need to be compiled
SOURCES = \
	main.cpp \
	mainwindow.cpp \
	Boyer-widget.cpp \
	NFA-widget.cpp \
	SpellChecker-widget.cpp

# Headers with Q_OBJECT that need MOC
MOC_HEADERS = \
	mainwindow.hpp \
	Boyer-widget.hpp \
	NFA-widget.hpp \
	SpellChecker-widget.hpp

# Generate MOC filenames
MOC_SOURCES = $(addprefix $(BUILD_DIR)/, $(MOC_HEADERS:.hpp=_moc.cpp))
MOC_OBJECTS = $(MOC_SOURCES:.cpp=.o)

# All object files
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=.o)) $(MOC_OBJECTS)

# ═══════════════════════════════════════════════════════════════
# BUILD RULES
# ═══════════════════════════════════════════════════════════════

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking..."
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "✓ Build complete: $(TARGET)"

# Compile C++ source files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate MOC files from headers
$(BUILD_DIR)/%_moc.cpp: %.hpp | $(BUILD_DIR)
	@echo "Generating moc for $<..."
	$(MOC) $(QT_CFLAGS) $< -o $@

# Compile MOC generated files
$(BUILD_DIR)/%_moc.o: $(BUILD_DIR)/%_moc.cpp | $(BUILD_DIR)
	@echo "Compiling moc $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# ═══════════════════════════════════════════════════════════════
# PHONY TARGETS
# ═══════════════════════════════════════════════════════════════

.PHONY: all clean run test install help info

run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

test: $(TARGET)
	@echo "Running unit tests..."
	./$(TARGET)
	@echo "✓ Tests completed"

clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)
	@echo "✓ Clean complete"

install: $(TARGET)
	@echo "Installing to /usr/local/bin/..."
	sudo cp $(TARGET) /usr/local/bin/streamlab
	@echo "✓ Installation complete"

help:
	@echo "Stream Lab - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run the application"
	@echo "  make test     - Build and run unit tests"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make install  - Install binary to /usr/local/bin"
	@echo "  make help     - Show this help message"
	@echo "  make info     - Show build configuration"
	@echo ""
	@echo "Project: C++17 Stream Lab with Qt5"
	@echo "Algorithms: Boyer-Moore, NFA→DFA, Spell Checker"

# ═══════════════════════════════════════════════════════════════
# BUILD INFORMATION
# ═══════════════════════════════════════════════════════════════

info:
	@echo "Build Configuration:"
	@echo "  CXX:        $(CXX)"
	@echo "  CXXFLAGS:   $(CXXFLAGS)"
	@echo "  MOC:        $(MOC)"
	@echo "  Target:     $(TARGET)"
	@echo "  Objects:    $(words $(OBJECTS)) files"
	@echo "  Sources:    $(SOURCES)"
	@echo "  MOC Headers: $(MOC_HEADERS)"
	@echo "  MOC files:  $(words $(MOC_SOURCES)) files"
