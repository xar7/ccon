CFLAGS += -Wall -Werror -Wextra -std=c99 -D DEBUG
OBJS = src/main.o
BIN = ccon

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
