# Makefile

# generic variables
CC=g++
CFLAGS=-g3
LDFLAGS=-lm -lasound

# build dir and target
BUILD_DIR := ../build
TARGET := $(BUILD_DIR)/cppTone

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
	$(CC) $(CFLAGS) -c $<


.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
