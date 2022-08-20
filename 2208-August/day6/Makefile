# Makefile

# generic variables
CC=g++
CFLAGS=-g3
LDFLAGS=-lm -lasound -lpulse -lpulse-simple -pthread

# build dir and target
BUILD_DIR := ../build
TARGET := $(BUILD_DIR)/modplayer

# source and object files
SRCS := $(shell find . -name '*.cpp')
OBJS := $(SRCS:%.cpp=%.o)

# Makefile rules
all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	@mkdir -p $@

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $^ -o $@


.PHONY: clean
clean:
	@find . -type f -name '*.o' -exec rm {} \;
	@rm -f $(TARGET)
