# Makefile для сборки тестовой программы MyTerm

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD

BUILD_DIR = ../obj/MyTermTest
SRC_DIR = ../MyTermTest
TEST_NAME = MyTermTest.o
LIB_DIR_MYTERM = ../obj/MyTerm
LIB_DIR_MYSIMPLECOMPUTER = ../obj/mySimpleComputer
LIB_DIR_CASH = ../obj/Cash
LIB_NAME_MYTERM = $(LIB_DIR_MYTERM)/MyTerm.a
LIB_NAME_MYSIMPLECOMPUTER = $(LIB_DIR_MYSIMPLECOMPUTER)/mySimpleComputer.a
LIB_NAME_CASH = $(LIB_DIR_CASH)/Cash.a

TEST_SOURCES = $(wildcard $(SRC_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEPS = $(TEST_OBJECTS:%.o=%.d)

.PHONY: all clean

all: $(BUILD_DIR)/$(TEST_NAME)

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIB_NAME_MYTERM):
	$(MAKE) -C ../MyTerm -f ../MyTerm/Makefile all

$(LIB_NAME_MYSIMPLECOMPUTER):
	$(MAKE) -C ../mySimpleComputer -f ../mySimpleComputer/Makefile all

$(LIB_NAME_CASH):
	$(MAKE) -C ../Cash -f ../Cash/Makefile all

clean:
	rm -f $(TEST_OBJECTS) $(DEPS) $(BUILD_DIR)/$(TEST_NAME)

