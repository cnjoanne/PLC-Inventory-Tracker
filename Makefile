## TODO: to fix
CC= gcc
CFLAGS = -Wall -Werror -ansi -pedantic -Iinclude  # -Iinclude is to allow *.c files to access header files from directory ./include

# directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR  = obj
INCLUDE_DIR = include

# source file and object file
SRC = $(SRC_DIR)/main.c  $(SRC_DIR)/csv_parser.c $(SRC_DIR)/binary_cache.c $(SRC_DIR)/sorting.c $(SRC_DIR)/filtering.c ## $(SRC_DIR)/latex_export.c $(SRC_DIR)/utils.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# output executable file
EXEC = $(BIN_DIR)/main.exe

# default target. Means, when we run 'make' with no arguments in terminal, computer looks for 'all'. Computer then builds the executable EXEC
all: $(EXEC)

# rule to build executable
# @mkdir -p $(BIN_DIR)
$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR) || @if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(OBJ) -o $(EXEC)

# rule to compile source file (*.c) to object files (*.o)
# @mkdir -p $(OBJ_DIR)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) || @if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# remove object files
# rm -rf $(OBJ_DIR)
# @if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR)/main.exe del /f /q $(BIN_DIR)\main.exe
