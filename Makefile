CC=g++
CC_FLAGS=-Wall -Werror -g

BUILD_DIR=Debug
SRC_DIR=src

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(notdir $(SRC_FILES:.cpp=.o))
OBJ_TEST = $(filter-out main.o, $(OBJ_FILES))
OBJECTS=$(patsubst %, Debug/%, $(OBJ_FILES))
OBJECTS_TEST = $(patsubst %, Debug/%, $(OBJ_TEST))

all: server tests

server: $(OBJECTS)
	$(CC) $(OBJECTS) -o server -lpthread

tests: $(OBJECTS_TEST) tests/tests.o	
	$(CC) $(OBJECTS_TEST) tests/tests.o -o server_tests -lpthread

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

tests/%.o: tests/%.cpp tests/ctb_test.h
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f server $(OBJECTS)