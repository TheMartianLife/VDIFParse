CC = gcc
CFLAGS = -Wall -Winline -pipe
LIBS = -lpthread -lvdifparse
PERMS = 0755

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

LIB_DIR = ./lib
LDFLAGS = -L$(LIB_DIR)
STATIC = libvdifparse.a

$(STATIC): $(OBJ)
	@echo "[Link (Static)] $^ > $@" 
	@ar rcs $@ $^

.c.o:
	@echo "[Compile] $<"
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) vdifparse_test vdifparse

.PHONY: install
install: $(STATIC)
	@echo "[Install] $< > $(LIB_DIR)"
	@install -m $(PERMS) $< $(LIB_DIR)

all: vdifparse

vdifparse: install vdifparse.c
	@echo "[Output] $@"
	$(CC) $(CFLAGS) -o $@ vdifparse.c $(LDFLAGS) $(LIBS)

test: vdifparse_test

vdifparse_test: install
	@echo "[Output] $@"
	$(CC) $(CFLAGS) -g -o $@ test/vdifparse_test.c $(LDFLAGS) $(LIBS)