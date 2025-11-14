CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =
TARGET = bank_manager
SRCS = main.c account.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Pattern rule: build any .o from its .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure objects rebuild if header changes
main.o: account.h
account.o: account.h

clean:
	rm -f $(OBJS) $(TARGET)
