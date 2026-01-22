.PHONY: all build run clean test help rebuild

# Compiler and flags
CXX := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -fPIC
DEBUG_FLAGS := -g -O0 -DDEBUG
CPPFLAGS := -I.

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
TEST_BUILD_DIR := $(BUILD_DIR)/tests
TEST_OBJ_DIR := $(TEST_BUILD_DIR)/obj

# Source files
SOURCES := $(shell find $(SRC_DIR)/chess -name "*.cpp" -not -name "main.cpp")
MAIN_SRC := $(SRC_DIR)/game.cpp
TEST_SOURCES := $(shell find tests -name "*.cpp")

# Object files
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
MAIN_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRC))
TEST_OBJECTS := $(patsubst tests/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SOURCES))

# Executables
EXECUTABLE := $(BIN_DIR)/chess
TEST_EXECUTABLE := $(TEST_BUILD_DIR)/chess_tests

# Default target
all: build

help:
	@echo "AFK2 Console Chess - C++ Makefile"
	@echo "===================================="
	@echo ""
	@echo "Targets:"
	@echo "  make build        - Build the chess program (default)"
	@echo "  make run          - Build and run the chess program"
	@echo "  make test         - Build and run unit tests"
	@echo "  make clean        - Remove all build artifacts"
	@echo "  make rebuild      - Clean and build from scratch"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make run          # Start the chess game"
	@echo "  make test         # Run all unit tests"

# Build the main executable
build: $(EXECUTABLE)
	@echo "✓ Build complete: $(EXECUTABLE)"

$(EXECUTABLE): $(OBJECTS) $(MAIN_OBJ) | $(BIN_DIR)
	@echo "Linking $(EXECUTABLE)..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^
	@echo "✓ Executable created successfully"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Run the program
run: build
	@echo ""
	@echo "Starting AFK2 Console Chess..."
	@echo "=============================="
	@echo ""
	@$(EXECUTABLE)

# Build and run tests
test: $(TEST_EXECUTABLE)
	@echo ""
	@echo "Running Unit Tests..."
	@echo "===================="
	@echo ""
	@$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(OBJECTS) $(TEST_OBJECTS) | $(TEST_BUILD_DIR)
	@echo "Linking test executable..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^
	@echo "✓ Test executable created"

$(TEST_OBJ_DIR)/%.o: tests/%.cpp | $(TEST_OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling test $<..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Create necessary directories
$(BIN_DIR) $(OBJ_DIR) $(TEST_OBJ_DIR) $(TEST_BUILD_DIR):
	@mkdir -p $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@echo "✓ Clean complete"

# Rebuild from scratch
rebuild: clean build
	@echo "✓ Rebuild complete"

# Debug build with symbols
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean build
	@echo "✓ Debug build complete"

# Print variables (useful for debugging Makefile)
.PHONY: print-vars
print-vars:
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "TEST_SOURCES: $(TEST_SOURCES)"
	@echo "TEST_OBJECTS: $(TEST_OBJECTS)"
	@echo "EXECUTABLE: $(EXECUTABLE)"
	@echo "TEST_EXECUTABLE: $(TEST_EXECUTABLE)"
