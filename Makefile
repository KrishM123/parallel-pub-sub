# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -pthread

# Directories
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

# Target executable
TARGET := main

# Source files and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Test files (optional)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))
TEST_TARGET := test_exec

# Default target
all: $(TARGET)

# Link the main executable
$(TARGET): $(OBJS) main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) main.cpp

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up the build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

# Run the main program
run: $(TARGET)
	./$(TARGET)