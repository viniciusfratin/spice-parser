CC = gcc
CC_OPTIONS = -g

SRC_DIR = src
BIN_DIR = bin
TMP_DIR = tmp
INCLUDE_DIR = include

INCLUDE_DIRS = -I$(TMP_DIR) -I$(INCLUDE_DIR)

FLEX_INPUT_FILE = $(SRC_DIR)/lexer.l
FLEX_OUTPUT_SOURCE_FILE = $(TMP_DIR)/lexer.c
FLEX_OUTPUT_HEADER_FILE = $(TMP_DIR)/lexer.h

BISON_INPUT_FILE = $(SRC_DIR)/parser.y
BISON_OUTPUT_SOURCE_FILE = $(TMP_DIR)/parser.c
BISON_OUTPUT_HEADER_FILE = $(TMP_DIR)/parser.h

SRC_INPUT_FILES = $(SRC_DIR)/generic_list.c $(SRC_DIR)/label_list.c $(SRC_DIR)/element_list.c $(SRC_DIR)/string_operations.c $(SRC_DIR)/spice_format.c $(SRC_DIR)/matrix_operations.c $(SRC_DIR)/mna.c $(SRC_DIR)/build_matrices.c $(SRC_DIR)/lu_decomposition.c $(SRC_DIR)/lu_solve.c $(SRC_DIR)/iterative_refinement.c $(SRC_DIR)/main.c

OUTPUT_FILE = $(BIN_DIR)/spice-parser

all:
	flex --header-file=$(FLEX_OUTPUT_HEADER_FILE) --outfile=$(FLEX_OUTPUT_SOURCE_FILE) $(FLEX_INPUT_FILE)
	bison --output-file=$(BISON_OUTPUT_SOURCE_FILE) --defines=$(BISON_OUTPUT_HEADER_FILE) --warnings=all --feature=all $(BISON_INPUT_FILE)
	$(CC) $(CC_OPTIONS) $(FLEX_OUTPUT_SOURCE_FILE) $(BISON_OUTPUT_SOURCE_FILE) $(SRC_INPUT_FILES) $(INCLUDE_DIRS) -o $(OUTPUT_FILE)$(OUTPUT_EXTENSION)
	
clean:
	rm $(TMP_DIR)/* $(OUTPUT_FILE)
