SRC_DIR := src
OBJ_DIR := obj
LIB_DIR := lib
LIB_DIRS :=  $(wildcard $(LIB_DIR)/*/)
LIB_FILES :=  $(wildcard $(LIB_DIR)/*/*.cpp)
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) $(foreach lib_dir,$(LIB_DIRS),$(patsubst $(lib_dir)%.cpp,$(OBJ_DIR)/%.o,$(wildcard $(lib_dir)*.cpp))) $(foreach lib_dir,$(LIB_DIRS),$(patsubst $(lib_dir)%.c,$(OBJ_DIR)/%.o,$(wildcard $(lib_dir)*.c)))
H_DIR := include/Headers
LIB_I := $(foreach lib_dir,$(LIB_DIRS), -I./$(lib_dir))
LIB_FLAGS := $(foreach lib_dir,$(LIB_DIRS), -L$(lib_dir))
LDFLAGS := -lGLEW -lglfw -lGL 
CPPFLAGS := -Wall -g -std=c++11 -I./$(H_DIR)
#CXXFLAGS := 

main: $(OBJ_FILES)
	   g++ -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(H_DIR)/%.h 
	   g++ $(LIB_I) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_DIR)/*/%.cpp
	g++ $(LIB_I) $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_DIR)/*/%.c
	gcc $(LIB_I) $(CPPFLAGS) -c -o $@ $<

clean: 
	rm $(OBJ_DIR)/*
debug:
	@echo $(LIB_I)
	@echo $(LIB_FLAGS)
