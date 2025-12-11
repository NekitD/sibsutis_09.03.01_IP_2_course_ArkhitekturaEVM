# Главный Makefile

CC = gcc
CFLAGS = -Wall -g -I../include
CPPFLAGS = -MP -MMD
LDFLAGS = -L../obj/mySimpleComputer -L../obj/MyTerm -L../obj/myBigChars -lm -L../obj/myReadkey -L../obj/CU -L../obj/cash

OBJ_DIR = ../obj
BUILD_DIR_LIB_MYSIMPLECOMPUTER = $(OBJ_DIR)/mySimpleComputer
BUILD_DIR_LIB_MYTERM = $(OBJ_DIR)/MyTerm
BUILD_DIR_LIB_MYBIGCHARS = $(OBJ_DIR)/myBigChars
BUILD_DIR_LIB_MYKEY = $(OBJ_DIR)/myReadkey
BUILD_DIR_LIB_CU = $(OBJ_DIR)/CU
BUILD_DIR_TEST_PR01 = $(OBJ_DIR)/SimpleComputerTest
BUILD_DIR_TEST_CONSOLE = $(OBJ_DIR)/MyTermTest
BUILD_DIR_FONT = $(OBJ_DIR)/font
BUILD_DIR_CASH = $(OBJ_DIR)/cash
EXE_DIR = .
SAVES_DIR = ./saves

FONT_FILE = $(EXE_DIR)/font.bin

PATH_SIMPLEASSMBLER = ../SimpleAssembler
SIMPLEASSEMBLER_EXE = ../console/SimpleAssembler.exe

LIB_NAME_MYSIMPLECOMPUTER = mySimpleComputer.a
LIB_NAME_MYTERM = MyTerm.a
LIB_NAME_MYBIGCHARS = myBigChars.a
LIB_NAME_MYKEY = myReadkey.a
LIB_NAME_CU = CU.a
LIB_NAME_CASH = сash.a

LIB_PATH_MYSIMPLECOMPUTER = $(BUILD_DIR_LIB_MYSIMPLECOMPUTER)/$(LIB_NAME_MYSIMPLECOMPUTER)
LIB_PATH_MYTERM = $(BUILD_DIR_LIB_MYTERM)/$(LIB_NAME_MYTERM)
LIB_PATH_MYBIGCHARS = $(BUILD_DIR_LIB_MYBIGCHARS)/$(LIB_NAME_MYBIGCHARS)
LIB_PATH_MYKEY = $(BUILD_DIR_LIB_MYKEY)/$(LIB_NAME_MYKEY)
LIB_PATH_CU = $(BUILD_DIR_LIB_CU)/$(LIB_NAME_CU)
LIB_PATH_CASH = $(BUILD_DIR_CASH)/$(LIB_NAME_CASH)

TEST_OBJECTS_PR01 = $(BUILD_DIR_TEST_PR01)/pr01.o
TEST_OBJECTS_CONSOLE = $(BUILD_DIR_TEST_CONSOLE)/MyTermTest.o
FONT_OBJ = $(BUILD_DIR_FONT)/font.o

.DEFAULT_GOAL := all

.PHONY: run-pr01 run-console run-sa all clean

$(LIB_PATH_MYSIMPLECOMPUTER):
	$(MAKE) -C ../mySimpleComputer

$(LIB_PATH_MYTERM):
	$(MAKE) -C ../MyTerm

$(TEST_OBJECTS_PR01):
	$(MAKE) -C ../SimpleComputerTest

$(TEST_OBJECTS_CONSOLE):
	$(MAKE) -C ../MyTermTest

$(LIB_PATH_MYBIGCHARS):
	$(MAKE) -C ../myBigChars

$(LIB_PATH_MYKEY):
	$(MAKE) -C ../myReadkey

$(LIB_PATH_CASH):
	$(MAKE) -C ../Cash

$(LIB_PATH_CU):
	$(MAKE) -C ../CU

$(FONT_OBJ):
	$(MAKE) -C ../font

$(SIMPLEASSEMBLER_EXE):
	$(MAKE) -C ../SimpleAssembler

$(EXE_DIR)/pr01.exe: $(TEST_OBJECTS_PR01) $(LIB_PATH_MYSIMPLECOMPUTER)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $(EXE_DIR)/pr01.exe $(LDFLAGS)

$(EXE_DIR)/console.exe: $(TEST_OBJECTS_CONSOLE) $(LIB_PATH_MYSIMPLECOMPUTER) $(LIB_PATH_MYTERM) $(LIB_PATH_MYBIGCHARS) $(LIB_PATH_MYKEY) $(LIB_PATH_CU) $(LIB_PATH_CASH)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $(EXE_DIR)/console.exe $(LDFLAGS)

$(FONT_FILE): $(EXE_DIR)/font.exe
	$(EXE_DIR)/font.exe

$(EXE_DIR)/font.exe: $(FONT_OBJ) $(LIB_PATH_MYBIGCHARS) $(LIB_PATH_MYTERM)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $(EXE_DIR)/font.exe $(LDFLAGS)


run-pr01: $(EXE_DIR)/pr01.exe
	./$(EXE_DIR)/pr01.exe

run-console: $(EXE_DIR)/console.exe
	mkdir -p ./saves
	./$(EXE_DIR)/console.exe

sat: $(SIMPLEASSEMBLER_EXE)
	@INPUT="$(word 2, $(MAKECMDGOALS))"; \
	OUTPUT="$(word 3, $(MAKECMDGOALS))"; \
	if [ -z "$$INPUT" ] || [ -z "$$OUTPUT" ]; then \
		echo "Usage: make sat input.sa output.bin"; \
		echo "Note: input.sa should be in saves/ folder"; \
		exit 1; \
	fi; \
	if [ ! -f "./saves/$$INPUT" ]; then \
		echo "Error: Input file ./saves/$$INPUT not found"; \
		exit 1; \
	fi; \
	mkdir -p ../console/saves; \
	./$(SIMPLEASSEMBLER_EXE) ./saves/$$INPUT ./saves/$$OUTPUT
%:
	@:

all: $(EXE_DIR)/pr01.exe $(EXE_DIR)/console.exe $(EXE_DIR)/font.exe $(FONT_FILE) $(SIMPLEASSEMBLER_EXE)

clean:
	$(MAKE) -C ../mySimpleComputer clean
	$(MAKE) -C ../SimpleComputerTest clean
	$(MAKE) -C ../SimpleAssembler clean
	$(MAKE) -C ../MyTerm clean
	$(MAKE) -C ../MyTermTest clean
	$(MAKE) -C ../myBigChars clean
	$(MAKE) -C ../myReadkey clean
	$(MAKE) -C ../CU clean
	$(MAKE) -C ../font clean
	$(MAKE) -C ../Cash clean
	rm -f $(EXE_DIR)/*.bin
	rm -f $(EXE_DIR)/*.exe
	rm -f $(SAVES_DIR)/*.bin

