CFLAGS += -Wall -Werror -Wextra -std=c99 -g

OBJS = src/main.o src/container.o
BIN = ccon

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(BIN)
