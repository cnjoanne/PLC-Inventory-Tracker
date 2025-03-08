## TODO: to fix
CC= gcc
CFLAGS = -Wall -Werror -ansi -pedantic -Iinclude  # -Iinclude is to allow *.c files to access header files from directory ./include

# directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR  = obj
INCLUDE_DIR = include

# source file and object file 
SRC = $(SRC_DIR)/main.c  $(SRC_DIR)/csv_parser.c ## $(SRC_DIR)/filtering.c $(SRC_DIR)/sorting.c $(SRC_DIR)/latex_export.c $(SRC_DIR)/utils.c $(SRC_DIR)/binary_cache.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# output executable file
EXEC = $(BIN_DIR)/main.exe

# default target. Means, when we run 'make' with no arguments in terminal, computer looks for 'all'. Computer then builds the executable EXEC
all: $(EXEC)

# rule to build executable 
## (can refer to the slide MAKEFILE taught in week 6 class 2)
$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(EXEC)

# rule to compile source file (*.c) to object files (*.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  ## creates obj directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# remove object files
clean:
	rm -rf $(OBJ_DIR)
