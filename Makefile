OS := $(shell uname 2>/dev/null || echo Windows)

CC= gcc
CFLAGS = -Wall -Werror -ansi -pedantic -Iinclude  # -Iinclude is to allow *.c files to access header files from directory ./include

# directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR  = obj
INCLUDE_DIR = include
CAC_DIR = data/cache

# source file and object file
SRC = $(SRC_DIR)/main.c  $(SRC_DIR)/csv_parser.c $(SRC_DIR)/binary_cache.c $(SRC_DIR)/sorting.c $(SRC_DIR)/filtering.c $(SRC_DIR)/instruction_printer.c $(SRC_DIR)/latex_export.c $(SRC_DIR)/utils.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# output executable file
EXEC = $(BIN_DIR)/main.exe

# default target. Means, when we run 'make' with no arguments in terminal, computer looks for 'all'. Computer then builds the executable EXEC
all: $(EXEC)

# rule to build executable
$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR) || @if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(OBJ) -o $(EXEC) -lregex

# rule to compile source file (*.c) to object files (*.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) || @if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# remove object files, executable files, and binary cache 
clean:
ifeq ($(OS),Windows)
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(BIN_DIR)\main.exe" del /f /q "$(BIN_DIR)\main.exe"
	@if exist "$(CAC_DIR)\bin_cache.bin" del /f /q "$(CAC_DIR)\bin_cache.bin"
else
	@if [ -d "$(OBJ_DIR)" ]; then rm -rf "$(OBJ_DIR)"; fi
	@if [ -f "$(BIN_DIR)/main.exe" ]; then rm -f "$(BIN_DIR)/main.exe"; fi
	@if [ -f "$(CAC_DIR)/bin_cache.bin" ]; then rm -f "$(CAC_DIR)/bin_cache.bin"; fi
endif

# clean latex reports in ./output/*.tex
clean-reports:
ifeq ($(OS),Windows)
	@if exist "output\*.tex" del /f /q output\*.tex
else
	@if ls output/*.tex 1> /dev/null 2>&1; then rm -f output/*.tex; fi
endif