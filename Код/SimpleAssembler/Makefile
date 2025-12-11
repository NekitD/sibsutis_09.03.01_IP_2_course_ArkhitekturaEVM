#Makefile для сборки объектных файлов SimpleAssembler

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD

OBJ_DIR = ../obj/SimpleAssembler
EXE_DIR = ../console
SRC_DIR = ../SimpleAssembler

EXE_NAME = SimpleAssembler.exe

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS = $(OBJECTS:%.o=%.d)

.PHONY: all clean

all: $(EXE_DIR)/$(EXE_NAME)

-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(EXE_DIR)/$(EXE_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Очистка
clean:
	rm -rf $(OBJECTS) $(DEPS) $(EXE_DIR)/$(EXE_NAME)