CC = g++
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES = $(OBJ_FILES:.o=.d)

PROGRAMS = preprocessor montador

.PHONY: all
all: setup $(PROGRAMS)

.PHONY: setup
setup:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

preprocessor: $(OBJ_DIR)/preprocessor.o $(OBJ_DIR)/utils.o
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@

montador: $(OBJ_DIR)/montador.o $(OBJ_DIR)/tables.o $(OBJ_DIR)/preprocessor.o
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -MMD -MP $< -o $@

-include $(DEP_FILES)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(addprefix $(BIN_DIR)/,$(PROGRAMS))