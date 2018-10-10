CC=g++
CFLAGS=-Wall Werror

BUILD_DIR = ./Debug/

SRC_FILES = $(wildcard *.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

server: $(BUILD_DIR)$(OBJ_FILES)
	$(CC) $(BUILD_DIR)$(OBJ_FILES) -o server

$(BUILD_DIR)%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f server $(BUILD_DIR)$(OBJ_FILES)
