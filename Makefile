UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	SHARED_EXT := .so
	STATIC_EXT := .a
	TEST_EXEC_EXT := .out
	LDLIBS := -lglfw -lm -lfreetype
	LM := -lm
	TEST_RPATH := -Wl,-rpath='$$ORIGIN'
endif
ifeq ($(UNAME_S),Darwin)
	SHARED_EXT := .dylib
	STATIC_EXT := .a
	TEST_EXEC_EXT := .app
	LDLIBS := -lglfw -lfreetype
endif
ifeq ($(OS),Windows_NT)
    SHARED_EXT := .dll
    STATIC_EXT := .lib
	TEST_EXEC_EXT := .exe
	LDLIBS := -lglfw3 -lfreetype
endif

CC := gcc

BUILD_DIR := build

CFLAGS := -Wall \
	-Wextra \
	-Werror \
	-Iinclude \
	$(foreach dir,$(wildcard vendor/*/include),-I$(dir)) \
	-fPIC \
	-std=c11

TEST_CFLAGS := -Wall -Wextra -Werror -Iinclude -std=c11

LIB_NAME := moxie

SHARED_LIB := $(BUILD_DIR)/lib$(LIB_NAME)$(SHARED_EXT)

STATIC_LIB := $(BUILD_DIR)/lib$(LIB_NAME)$(STATIC_EXT)

TEST_SRC := test/test.c

TEST_BIN := $(BUILD_DIR)/test$(TEST_EXEC_EXT)

VENDOR_SRC := vendor/glad/src/glad.c

SRC := $(shell find src -name '*.c') $(VENDOR_SRC)

OBJ := $(patsubst %.c, $(BUILD_DIR)/obj/%.o, $(SRC))

all: $(STATIC_LIB) $(SHARED_LIB)

$(SHARED_LIB): $(OBJ)
	$(CC) -shared -o $@ $^ $(LDLIBS)

$(STATIC_LIB): $(OBJ)
	ar rcs $@ $^

shared: $(SHARED_LIB)
static: $(STATIC_LIB)

$(BUILD_DIR)/obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: shared
	$(CC) $(TEST_SRC) \
	-Iinclude \
	-L$(BUILD_DIR) \
	$(TEST_CFLAGS) \
	$(TEST_RPATH) \
	-l$(LIB_NAME) \
	$(LM) \
	-o $(TEST_BIN)

test-run: test
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)/