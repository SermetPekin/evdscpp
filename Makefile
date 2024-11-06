# Makefile for evdscpp
BUILD_DIR := build

# Compiler options
CXX := g++
CXXFLAGS := -std=c++20

# Default target
.PHONY: all
all: configure build

# Configure the project using CMake
.PHONY: configure
configure:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..

# Build the project
.PHONY: build
build: configure
	cd $(BUILD_DIR) && make

# Run the executable
.PHONY: run
run: build
	./$(BUILD_DIR)/bin/evdscpp
	./$(BUILD_DIR)/bin/evdscpp ./example.txt --cache true


# Run tests
.PHONY: test
test: build
	cd $(BUILD_DIR) && ctest --output-on-failure

# Clean the build directory
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Valgrind memory check (requires Valgrind)
.PHONY: valgrind
valgrind: build
	if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(BUILD_DIR)/bin/evdscpp; \
	else \
		echo "Valgrind not found. Skipping memory check."; \
	fi
