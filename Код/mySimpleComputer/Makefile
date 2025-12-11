# Makefile для сборки библиотеки mySimpleComputer

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD

BUILD_DIR = ../obj/mySimpleComputer
SRC_DIR = ../mySimpleComputer
LIB_NAME = mySimpleComputer.a
LIB_PATH = $(BUILD_DIR)/$(LIB_NAME)

MYTERM_SRC_DIR = ../MyTerm
MYTERM_BUILD_DIR = ../obj/MyTerm
MYTERM_LIB_PATH = $(MYTERM_BUILD_DIR)/MyTerm.a

MYKEY_SRC_DIR = ../myReadkey
MYKEY_BUILD_DIR = ../obj/myReadkey
MYKEY_LIB_PATH = $(MYKEY_BUILD_DIR)/myReadkey.a

MYBIGCHARS_SRC_DIR = ../myBigChars
MYBIGCHARS_BUILD_DIR = ../obj/myBigChars
MYBIGCHARS_LIB_PATH = $(MYBIGCHARS_BUILD_DIR)/myBigChars.a

LIB_SOURCES = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEPS = $(LIB_OBJECTS:%.o=%.d)

.PHONY: all clean

all: $(LIB_PATH)

$(LIB_PATH): $(LIB_OBJECTS) $(MYTERM_LIB_PATH) $(MYBIGCHARS_LIB_PATH) $(MYKEY_LIB_PATH)
	ar rcs $@ $^

$(MYBIGCHARS_LIB_PATH):
	$(MAKE) -C $(MYBIGCHARS_SRC_DIR) -f ../myBigChars/Makefile all

$(MYTERM_LIB_PATH):
	$(MAKE) -C $(MYTERM_SRC_DIR) -f ../MyTerm/Makefile all

$(MYKEY_LIB_PATH):
	$(MAKE) -C $(MYKEY_SRC_DIR) -f ../myReadkey/Makefile all

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p ../obj/mySimpleComputer
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJECTS) $(DEPS) $(LIB_PATH)
	$(MAKE) -C ../MyTerm clean
	$(MAKE) -C ../myBigChars clean
	$(MAKE) -C ../myReadkey clean
