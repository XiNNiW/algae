#makefile for algae

#project name
TARGET      = libalgae.a
TEST_TARGET = algae_test

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
#TEST_FRAMEWORK = gtest

#files
SOURCES  := $(wildcard $(SRC)/*.$(CODE_EXT))
INCLUDES := $(wildcard $(SRC))
OBJECTS  := $(SOURCES:$(SRC)/%.$(CODE_EXT)=$(OBJ)/%.o)

TEST_SOURCES := $(wildcard $(TEST_SRC)/*.$(CODE_EXT))
TEST_INCLUDES := $(INCLUDES) -Igtest/include
TEST_OBJECTS := $(TEST_SOURCES:$(TEST_SRC)/%.$(CODE_EXT)=$(TEST_OBJ)/%.o)

#compiler and settings
CC  = g++

CFLAGS = -std=c++17 -Wall -I. -I$(INCLUDE)
CFLAGS_TEST = -std=c++17 -Wall -I. -lm -Lgtest/lib -lfftw3 -I$(TEST_INCLUDES) -L./$(BIN) -lalgae

#linker and settings
LINKER = g++
# LFLAGS = -Wall -I. -lm -lc -I$(INCLUDES)
TEST_LFLAGS = -Wall -I. -lm -Lgtest/lib -lgtest_main -lgtest -lfftw3 -I$(TEST_INCLUDES) -L./$(BIN) -lalgae

#archiver and settings
AR  = ar
AR_OPTS = rvs


#targets
.PHONY: all
all:	$(BIN)/$(TARGET)

test:   clean all $(BIN)/$(TEST_TARGET)
		./$(BIN)/$(TEST_TARGET)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(BIN)/$(TARGET) $(BIN)/$(TEST_TARGET) $(TEST_OBJECTS) 
	echo "Workspace clean!"

$(BIN)/$(TARGET): $(OBJECTS)
	$(AR) $(AR_OPTS) $@ $^
	@echo "created archive!"

# $(BIN)/$(TARGET): $(OBJECTS)
# 	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
# 	@echo "Linking complete!"

$(BIN)/$(TEST_TARGET): $(TEST_OBJECTS)
	$(LINKER) $(TEST_OBJECTS) $(TEST_LFLAGS) -o $@
	@echo "Test linking complete!"

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.$(CODE_EXT)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled sucessfully!"

$(TEST_OBJECTS): $(TEST_OBJ)/%.o : $(TEST_SRC)/%.$(CODE_EXT)
	$(CC) $(CFLAGS_TEST) -c $< -o $@
	@echo "Tests compiled sucessfully!"

