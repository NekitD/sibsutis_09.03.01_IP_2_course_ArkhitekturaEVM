#Makefile для сборки объектных файлов font

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD

BUILD_DIR = ../obj/font
SRC_DIR = ../font
LIB_DIR_MYBIGCHARS = ../obj/myBigChars
LIB_NAME_MYBIGCHARS = $(LIB_DIR_MYBIGCHARS)/myBigChars.a

TEST_NAME = font.o

TEST_SOURCES = $(wildcard $(SRC_DIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEPS = $(TEST_OBJECTS:%.o=%.d)

.PHONY: all clean

all: $(BUILD_DIR)/$(TEST_NAME)

-include $(DEPS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIB_NAME_MYBIGCHARS):
	$(MAKE) -C ../myBigChars -f ../myBigChars/Makefile all

clean:
	rm -f $(TEST_OBJECTS) $(DEPS) $(BUILD_DIR)/$(TEST_NAME)