# Default Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17 -D_POSIX_C_SOURCE=200809L

.PHONY: all clean test shell scheduler systems-lab help

# Default target
all: shell scheduler systems-lab

# Project modules
shell:
	@echo "==> Compiling Mini Shell..."
	$(MAKE) -C shell

scheduler:
	@echo "==> Compiling CPU Scheduler..."
	$(MAKE) -C scheduler

systems-lab:
	@echo "==> Compiling Linux Internals experiments..."
	$(MAKE) -C systems-lab

# Automated test execution
test:
	@echo "==> Running Mini Shell tests..."
	-$(MAKE) -C shell test
	@echo "==> Running CPU Scheduler tests..."
	-$(MAKE) -C scheduler test

# Cleaning generated binaries and objects
clean:
	@echo "==> Cleaning compiled files..."
	$(MAKE) -C shell clean
	$(MAKE) -C scheduler clean
	$(MAKE) -C systems-lab clean

# Help for Makefile commands
help:
	@echo "Available commands:"
	@echo "  make all          - Compiles all modules"
	@echo "  make shell        - Compiles only the Mini Shell"
	@echo "  make scheduler    - Compiles only the CPU simulator"
	@echo "  make systems-lab  - Compiles the Linux experiments"
	@echo "  make test         - Runs automated tests"
	@echo "  make clean        - Removes build artifacts"