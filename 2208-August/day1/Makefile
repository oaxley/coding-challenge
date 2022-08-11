# Makefile

# generic variables
CC=gcc
CFLAGS=-g
LDFLAGS=-lasound -lm

# build dir and target
BUILD_DIR := ../build
TARGET := $(BUILD_DIR)/tone

# source and object files
SRCS := $(shell find . -name '*.c')
OBJS := $(SRCS:%.c=%.o)

# Makefile rules
all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	@mkdir -p $@

$(TARGET): $(BUILD_DIR)/$(OBJS)
	$(CC) $< -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(TARGET)
