# Makefile для сборки тестовой программы mySimpleComputer

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD

BUILD_DIR = ../obj/SimpleComputerTest
SRC_DIR = ../SimpleComputerTest
TEST_NAME = pr01
LIB_DIR = ../obj/mySimpleComputer
LIB_NAME = $(LIB_DIR)/mySimpleComputer.a

TEST_SOURCES = $(wildcard $(SRC_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEPS = $(TEST_OBJECTS:%.o=%.d)

.PHONY: all clean

all: $(TEST_OBJECTS)

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p ../obj/SimpleComputerTest
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(TEST_OBJECTS) $(DEPS)