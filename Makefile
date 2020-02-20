CFLAGS += -Wall -Werror -Wextra -std=c99 -g

OBJS = src/main.o src/container.o
BIN = ccon
BUSYBOX_DL_URL = https://www.busybox.net/downloads/binaries/1.31.0-defconfig-multiarch-musl/busybox-x86_64

all: $(BIN) fs

fs:
	mkdir -p cconfs/{bin,boot,dev,etc,home,lib,mnt,opt,proc,root,run,sbin,sys,usr,var,tmp}
	curl $(BUSYBOX_DL_URL) -o cconfs/bin/busybox
	chmod +x cconfs/bin/busybox

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(BIN)
	$(RM) -rf cconfs busybox.tar
