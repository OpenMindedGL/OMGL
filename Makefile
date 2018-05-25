SRC_DIR := src
OBJ_DIR := obj
LIB_DIR := lib
LIB_DIRS :=  $(wildcard $(LIB_DIR)/*/)
LIB_FILES :=  $(wildcard $(LIB_DIR)/*/*.cpp)
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) $(foreach lib_dir,$(LIB_DIRS),$(patsubst $(lib_dir)%.cpp,$(OBJ_DIR)/%.o,$(wildcard $(lib_dir)*.cpp)))
H_DIR := include/Headers
LIB_I := $(foreach lib_dir,$(LIB_DIRS), -I./$(lib_dir))
CFLAGS = -Wall -g -std=c++11
LIBS = -lGLEW -lglfw -lGL -lglut
LDFLAGS := -lGLEW -lglfw -lGL -lglut
CPPFLAGS := -Wall -g -std=c++11 -I./$(H_DIR)
#CXXFLAGS := 

main: $(OBJ_FILES)
	   g++ -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	   g++ $(LIB_I) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LIB_DIR)/*/%.cpp
	g++ $(LIB_I) $(CPPFLAGS) -c -o $@ $<

clean: 
	rm $(OBJ_DIR)/*
debug:
	@echo $(LIB_I)
