CFLAGS += -Wall -Werror -Wextra -std=c99 -D DEBUG
OBJS = src/main.o src/container.o
BIN = ccon

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
