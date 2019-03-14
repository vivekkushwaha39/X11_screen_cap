################################################################
# 		@author Vivek Kushwaha <vivekkushwaha39@gmail.com>     #
################################################################
CC=g++

SRC_DIR=src/
INC_DIR=includes
BUILD_DIR=debug/

TARGET=$(BUILD_DIR)screencap.exe
SRC_FILES=$(shell find "src/" -name "*.cpp" -type f) 
OBJ_FILE_PATH=$(patsubst %.cpp, $(BUILD_DIR)%.o, $(SRC_FILES))

COMPILER_FLAGS= -O0 -ggdb -DDEBUG -I$(INC_DIR)

LIBS=-lX11 -lpng

$(BUILD_DIR)%.o: %.cpp
	@echo "Compiling $@ from $^" 
	$(CC) $(COMPILER_FLAGS)  -c $^ -o $@


all: PREBUILD_STEPS $(TARGET)

PREBUILD_STEPS:
	@echo "Making directories"
	$(shell find "src/" -name  "*" -type d -exec mkdir -p 'debug/{}' \; ) 




$(TARGET): $(OBJ_FILE_PATH)
	$(CC) $(OBJ_FILE_PATH) -o $(TARGET) $(LIBS)

clean: 
	@rm -r debug
print:
	@echo $(SRC_FILES)
	@echo $(OBJ_FILE_PATH)
