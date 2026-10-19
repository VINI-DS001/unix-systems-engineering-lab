CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17 -D_POSIX_C_SOURCE=200809L -Iinclude

SRC = src/main.c src/shell.c src/builtins.c
OBJ = $(SRC:.c=.o)
TARGET = minishell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

test: all
	@echo "Running basic shell test..."
	@echo "pwd" | ./$(TARGET)

.PHONY: all clean test