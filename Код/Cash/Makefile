# Makefile для сборки библиотеки cash

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD

BUILD_DIR = ../obj/cash
SRC_DIR = ../Cash
LIB_NAME = сash.a
LIB_PATH = $(BUILD_DIR)/$(LIB_NAME)

MYTERM_SRC_DIR = ../MyTerm
MYTERM_BUILD_DIR = ../obj/MyTerm
MYTERM_LIB_PATH = $(MYTERM_BUILD_DIR)/MyTerm.a

MYSIMPLECOMPUTER_SRC_DIR = ../mySimpleComputer
MYSIMPLECOMPUTER_BUILD_DIR = ../obj/mySimpleComputer
MYSIMPLECOMPUTER_LIB_PATH = $(MYSIMPLECOMPUTER_BUILD_DIR)/mySimpleComputer.a

LIB_SOURCES = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEPS = $(LIB_OBJECTS:%.o=%.d)

.PHONY: all clean

all: $(LIB_PATH)

$(LIB_PATH): $(LIB_OBJECTS) $(MYTERM_LIB_PATH) $(MYSIMPLECOMPUTER_LIB_PATH)
	ar rcs $@ $^

$(MYTERM_LIB_PATH):
	$(MAKE) -C $(MYTERM_SRC_DIR) -f ../MyTerm/Makefile all

$(MYSIMPLECOMPUTER_LIB_PATH):
	$(MAKE) -C $(MYSIMPLECOMPUTER_SRC_DIR) -f ../mySimpleComputer/Makefile all

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p ../obj/cash
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJECTS) $(DEPS) $(LIB_PATH)
	$(MAKE) -C ../MyTerm clean
	$(MAKE) -C ../mySimpleComputer clean