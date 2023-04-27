#makefile for algae

#project name
TARGET      = libalgae.a
TEST_TARGET = algae_test
INSTALL_DIR = /usr/local/lib/

#directories
BIN  	= bin
SRC  	= src
OBJ  	= $(BIN)
LIB  	= lib
INCLUDE = $(SRC)/include
TEST 	= test
TEST_SRC = $(TEST)
TEST_OBJ = $(BIN)
CODE_EXT = cpp

#dependencies
TEST_DEPENDENCIES_COMPILE = -Lgtest/lib -lfftw3
TEST_DEPENDENCIES_LINK = -Lgtest/lib -lgtest_main -lgtest -lfftw3

#files
SOURCES  := $(wildcard $(SRC)/*.$(CODE_EXT))
INCLUDES := $(wildcard $(SRC))
OBJECTS  := $(SOURCES:$(SRC)/%.$(CODE_EXT)=$(OBJ)/%.o)

TEST_PACKAGE :=**
TEST_SOURCES := $(wildcard $(TEST_SRC)/*.$(CODE_EXT))
TEST_DIRS := $(sort $(dir $(TEST_SOURCES) $(TEST_SRC)))
TEST_INCLUDES := $(INCLUDES) -Igtest/include
TEST_OBJECTS := $(TEST_SOURCES:$(TEST_SRC)/%.$(CODE_EXT)=$(TEST_OBJ)/%.o)
TEST_DIRS := $(TEST_SOURCES:$(TEST_SRC)/%.$(CODE_EXT)=$(TEST_OBJ)/%/../)

#compiler and settings
#CC  = g++
CC = clang
CC_VERSION = c++17

CFLAGS = -std=$(CC_VERSION) -Wall -I. -I$(INCLUDE) -lm -march=native -O2
CFLAGS_TEST = -std=$(CC_VERSION) -Wall -I. $(TEST_DEPENDENCIES_COMPILE) -I$(TEST_INCLUDES) -L./$(BIN) -lm -O2 -lalgae

#linker and settings
LINKER = g++
# LFLAGS = -Wall -I. -lm -lc -I$(INCLUDES)
TEST_LFLAGS = -Wall -I. $(TEST_DEPENDENCIES_LINK) -I$(TEST_INCLUDES) -L./$(BIN) -L./$(BIN)/$(TARGET) -lm -O2 -lalgae

#archiver and settings
AR  = ar
AR_OPTS = rvs


#targets
.PHONY: all
all:	$(BIN)/$(TARGET)

test:   clean all test_directories $(BIN)/$(TEST_TARGET)
		./$(BIN)/$(TEST_TARGET)

test_directories:
	$(shell mkdir -p $(TEST_DIRS))

install:
	cp $(BIN)/$(TARGET) $(INSTALL_DIR)
	cp $(SRC)/algae.h /usr/local/include

.PHONY: clean
clean:
	@rm -rf $(OBJECTS) $(BIN)/$(TARGET) $(BIN)/$(TEST_TARGET) $(TEST_OBJECTS)
	@echo "Workspace clean!"

$(BIN)/$(TARGET): $(OBJECTS)
	$(AR) $(AR_OPTS) $@ $^
	@echo "created archive!"

# $(BIN)/$(TARGET): $(OBJECTS)
# 	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
# 	@echo "Linking complete!"

$(BIN)/$(TEST_TARGET): $(TEST_OBJECTS)
	$(LINKER) $(TEST_OBJECTS) $(BIN)/$(TARGET) $(TEST_LFLAGS) -o $@

# @echo "Test linking complete!"

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.$(CODE_EXT)
	$(CC) $(CFLAGS) -c $< -o $@

# @echo "Compiled sucessfully!"

$(TEST_OBJECTS): $(TEST_OBJ)/%.o : $(TEST_SRC)/%.$(CODE_EXT)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

# echo "Tests compiled sucessfully!"

