# Makefile для сборки библиотеки myReadkey

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD

BUILD_DIR = ../obj/myReadkey
SRC_DIR = ../myReadkey
LIB_NAME = myReadkey.a
LIB_PATH = $(BUILD_DIR)/$(LIB_NAME)

LIB_SOURCES = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEPS = $(LIB_OBJECTS:%.o=%.d)

.PHONY: all clean

all: $(LIB_PATH)

-include $(DEPS)

$(LIB_PATH): $(LIB_OBJECTS) $(MYTERM_LIB_PATH)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJECTS) $(DEPS) $(LIB_PATH)