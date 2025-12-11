# Makefile для сборки библиотеки CU

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD

BUILD_DIR = ../obj/CU
SRC_DIR = ../CU
LIB_NAME = CU.a
LIB_PATH = $(BUILD_DIR)/$(LIB_NAME)

MY_SRC_DIR = ../mySimpleComputer
MY_LIB_NAME = mySimpleComputer.a
MY_LIB_PATH = $(BUILD_DIR)/$(LIB_NAME)

MYTERM_SRC_DIR = ../MyTerm
MYTERM_BUILD_DIR = ../obj/MyTerm
MYTERM_LIB_PATH = $(MYTERM_BUILD_DIR)/MyTerm.a

MYKEY_SRC_DIR = ../myReadkey
MYKEY_BUILD_DIR = ../obj/myReadkey
MYKEY_LIB_PATH = $(MYKEY_BUILD_DIR)/myReadkey.a

CASH_SRC_DIR = ../Cash
CASH_BUILD_DIR = ../obj/cash
CASH_LIB_PATH = $(CASH_BUILD_DIR)/сash.a

LIB_SOURCES = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEPS = $(LIB_OBJECTS:%.o=%.d)

.PHONY: all clean

all: $(LIB_PATH)

-include $(DEPS)

$(LIB_PATH): $(LIB_OBJECTS) $(MYTERM_LIB_PATH) $(MY_LIB_PATH) $(MYKEY_LIB_PATH) $(CASH_LIB_PATH)
	ar rcs $@ $^

$(MYTERM_LIB_PATH):
	$(MAKE) -C $(MYTERM_SRC_DIR)

$(CASH_LIB_PATH):
	$(MAKE) -C ../Cash

$(MYSIMPLECOMPUTER_LIB_PATH):
	$(MAKE) -C $(MYSIMPLECOMPUTER_SRC_DIR)

$(MYKEY_LIB_PATH):
	$(MAKE) -C $(MYKEY_SRC_DIR)
	
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJECTS) $(DEPS) $(LIB_PATH)