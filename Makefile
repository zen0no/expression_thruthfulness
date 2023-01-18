SRC_DIR = src
BISON_DIR = src/parse
BISON_GEN_DIR = $(BISON_DIR)/gen
INCLUDE_DIR = include
BISON_INCLUDE = $(INCLUDE_DIR)/parse
BUILD_DIR = build

BUILD_TARGET = main

DIST_TARGET = dist

SRCS = $(wildcard $(SRC_DIR)/*.c) $(BISON_GEN_DIR)/expression.lexer.c $(BISON_GEN_DIR)/expression.tab.c 

GCC_FLAGS = -Wall -I $(INCLUDE_DIR)
GCC = gcc

.PHONY: all clean release bison_codegen run

all: bison build build/app 


$(BISON_GEN_DIR)/%.lexer.c $(BISON_GEN_DIR)/%.lexer.h: $(BISON_DIR)/%.lex $(BISON_GEN_DIR) $(BISON_INCLUDE)
	flex $(BISON_DIR)/$*.lex
	mv lex.c $(BISON_GEN_DIR)/$*.lexer.c
	mv lex.h $(BISON_INCLUDE)/$*.lexer.h

$(BISON_GEN_DIR)/%.tab.c $(BISON_GEN_DIR)/%.tab.h: $(BISON_DIR)/%.y $(BISON_GEN_DIR) $(BISON_INCLUDE)
	bison -d -v $< -o $(BISON_GEN_DIR)/$*.tab.c
	mv $(BISON_GEN_DIR)/$*.tab.h $(BISON_INCLUDE)/$*.tab.h

$(BISON_GEN_DIR):
	mkdir -p $(BISON_GEN_DIR)

$(BISON_INCLUDE):
	mkdir -p $(BISON_INCLUDE)

build/app: $(SRCS)
	$(GCC) $(GCC_FLAGS) -o $(BUILD_DIR)/$(BUILD_TARGET) $^

bison: $(BISON_GEN_DIR)/expression.lexer.c $(BISON_GEN_DIR)/expression.tab.c


run:
	./$(BUILD_DIR)/$(BUILD_TARGET)


build:
	mkdir -p build

dist:
	mkdir -p dist

clean:
	rm -rf build
	rm -rf dist
	rm -rf $(BISON_GEN_DIR)
	rm -rf $(BISON_INCLUDE)

release: clean all dist
	zip dist/$(DIST_TARGET) -r Makefile src include
