CC = clang
CFLAGS = -O2 -Wall -Wpedantic -Wextra -ggdb -std=c18
LIB_CFLAGS = -I../libs
LIB_SRC_DIR = ../libs/src
LIB_OBJ_DIR = ../libs/obj
LIB_SRCS = $(wildcard $(LIB_SRC_DIR)/*.c)
LIB_OBJS = $(patsubst $(LIB_SRC_DIR)/%.c, $(LIB_OBJ_DIR)/%.o, $(LIB_SRCS))
LIBRARY = ../libs/libAOC.a
CINCLUDES = -I/home/smolloy/Code/AOC2023/libs $(LIB_CFLAGS)
CLIBS = -L../libs -lAOC

SRC = src
OBJ = obj

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

BINDIR = bin
BIN = $(BINDIR)/main

all: $(LIBRARY) $(BIN)

$(BIN): $(OBJS) | $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CINCLUDES) $^ -o $@ $(CLIBS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CINCLUDES) -c $< -o $@

$(LIBRARY): $(LIB_OBJS) | $(LIB_OBJ_DIR)
	ar rcs $@ $^

$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.c | $(LIB_OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LIB_CFLAGS) -c $< -o $@

$(OBJ) $(LIB_OBJ_DIR):
	@mkdir -p $@

clean:
	rm -rf $(BINDIR) $(OBJ) $(LIB_OBJ_DIR) $(LIBRARY)

