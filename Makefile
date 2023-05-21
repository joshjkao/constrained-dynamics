SRC_DIR = src
BUILD_DIR = bin
CC=g++
CCFLAGS=-g -Wall -std=c++11
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -I include 
LIBRARY_PATHS = -L lib
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

OBJS = $(BUILD_DIR)/app.o $(BUILD_DIR)/constraint.o $(BUILD_DIR)/doublependulum.o $(BUILD_DIR)/forcegenerator.o $(BUILD_DIR)/matrix.o \
	$(BUILD_DIR)/object.o $(BUILD_DIR)/ODEsolver.o $(BUILD_DIR)/system.o $(BUILD_DIR)/texturemanager.o $(BUILD_DIR)/vector2d.o

all: $(BUILD_DIR)/.dirstamp $(BUILD_DIR)/play run
$(BUILD_DIR)/play: $(BUILD_DIR)/app.o $(SRC_DIR)/main.cpp $(OBJS)
	$(CC) $(CCFLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(OBJS)  $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/$(OBJ_NAME)
$(BUILD_DIR)/app.o: $(BUILD_DIR)/system.o $(BUILD_DIR)/ODEsolver.o $(BUILD_DIR)/doublependulum.o $(SRC_DIR)/app.cpp $(SRC_DIR)/app.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/app.cpp
$(BUILD_DIR)/constraint.o: $(BUILD_DIR)/object.o $(SRC_DIR)/constraint.cpp $(SRC_DIR)/constraint.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/constraint.cpp
$(BUILD_DIR)/doublependulum.o: $(BUILD_DIR)/vector2d.o $(SRC_DIR)/doublependulum.cpp $(SRC_DIR)/doublependulum.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/doublependulum.cpp
$(BUILD_DIR)/forcegenerator.o: $(BUILD_DIR)/object.o $(SRC_DIR)/system.h $(SRC_DIR)/forcegenerator.cpp $(SRC_DIR)/forcegenerator.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/forcegenerator.cpp
$(BUILD_DIR)/matrix.o: $(SRC_DIR)/matrix.cpp $(SRC_DIR)/matrix.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/matrix.cpp
$(BUILD_DIR)/object.o: $(BUILD_DIR)/vector2d.o $(SRC_DIR)/system.h $(SRC_DIR)/object.cpp $(SRC_DIR)/object.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/object.cpp
$(BUILD_DIR)/ODEsolver.o: $(BUILD_DIR)/system.o $(SRC_DIR)/ODEsolver.cpp $(SRC_DIR)/ODEsolver.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/ODEsolver.cpp
$(BUILD_DIR)/system.o: $(BUILD_DIR)/object.o $(BUILD_DIR)/forcegenerator.o $(BUILD_DIR)/constraint.o $(BUILD_DIR)/matrix.o $(SRC_DIR)/system.cpp $(SRC_DIR)/system.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/system.cpp
$(BUILD_DIR)/texturemanager.o: $(BUILD_DIR)/app.o $(SRC_DIR)/texturemanager.cpp $(SRC_DIR)/texturemanager.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/texturemanager.cpp
$(BUILD_DIR)/vector2d.o: $(SRC_DIR)/vector2d.cpp $(SRC_DIR)/vector2d.h
	$(CC) $(CCFLAGS) $(INCLUDE_PATHS) -o $@ -c $(SRC_DIR)/vector2d.cpp


run:
	$(BUILD_DIR)/play

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/.dirstamp:
	mkdir -p $(BUILD_DIR)
	touch $(BUILD_DIR)/.dirstamp