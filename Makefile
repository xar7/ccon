CC ?= clang
CFLAGS += -Wall -Werror -Wextra -std=c99 -g

OBJS = src/main.o src/container.o src/cgroup.o
BIN = ccon
BUSYBOX_DL_URL = https://www.busybox.net/downloads/binaries/1.31.0-defconfig-multiarch-musl/busybox-x86_64
FS=cconfs

all: $(BIN) $(FS)

$(FS):
	mkdir -p $(FS)/{bin,boot,dev,etc,home,lib,mnt,opt,proc,root,run,sbin,sys/fs/cgroup2,usr/{sbin,bin,lib},var,tmp}
	curl $(BUSYBOX_DL_URL) -o $(FS)/bin/busybox
	chmod +x $(FS)/bin/busybox

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(BIN)
	$(RM) -rf cconfs busybox.tar
