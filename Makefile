TARGET     := fbkeyboard

CXX        := g++
CXXFLAGS   := -std=c++14 -Wall 

SRC_DIR    := src
BIN_DIR    := bin
OBJ_DIR    := obj

SOURCES    := ${wildcard $(SRC_DIR)/*.cpp}
OBJECTS    := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: directories $(BIN_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo CC $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@echo LD $@
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean

.PHONY: directories

clean:
	-rm -rfv $(OBJ_DIR) $(BIN_DIR)

directories:
	@mkdir -p ./bin
	@mkdir -p ./obj

