CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17 -D_POSIX_C_SOURCE=200809L -Iinclude

SRCDIR = src
OBJDIR = obj
TARGET = scheduler_sim

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Scheduler: Cleanup completed."

test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test