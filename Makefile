PROJECT_NAME := CG2020

BUILD_DIR := build

# Define the names of key files
# SOURCE_FILE := src/$(PROJECT_NAME).cpp
# OBJECT_FILE := $(BUILD_DIR)/$(PROJECT_NAME).o

SOURCE_FILES := $(shell find src/ -type f -name '*.cpp') #wildcard src/*.cpp)
OBJECT_FILES := $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_FILES))
EXECUTABLE := $(BUILD_DIR)/$(PROJECT_NAME)
SDW_DIR := $(PWD)/libs/sdw/
GLM_DIR := $(PWD)/libs/glm-0.9.7.2/
SDW_SOURCE_FILES := $(wildcard $(SDW_DIR)*.cpp)
SDW_OBJECT_FILES := $(patsubst $(SDW_DIR)%.cpp, $(BUILD_DIR)/%.o, $(SDW_SOURCE_FILES))

# Build settings
COMPILER := clang++
COMPILER_OPTIONS := -c -pipe -Wall -std=c++11 # If you have an older compiler, you might have to use -std=c++0x
DEBUG_OPTIONS := -ggdb -g3
FUSSY_OPTIONS := -Werror -pedantic
SANITIZER_OPTIONS := -O1 -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer
SPEEDY_OPTIONS := -Ofast -funsafe-math-optimizations -march=native
LINKER_OPTIONS :=

# Set up flags
SDW_COMPILER_FLAGS := -I$(SDW_DIR)
GLM_COMPILER_FLAGS := -I$(GLM_DIR)
# If you have a manual install of SDL, you might not have sdl2-config installed, so the following line might not work
# Compiler flags should look something like: -I/usr/local/include/SDL2 -D_THREAD_SAFE
SDL_COMPILER_FLAGS := $(shell sdl2-config --cflags)
# If you have a manual install of SDL, you might not have sdl2-config installed, so the following line might not work
# Linker flags should look something like: -L/usr/local/lib -lSDL2
SDL_LINKER_FLAGS := $(shell sdl2-config --libs)
SDW_LINKER_FLAGS := $(SDW_OBJECT_FILES)

SELECTED_OPTIONS :=

default: debug

# Rule to compile and link for use with a debugger (although works fine even if you aren't using a debugger !)
debug: override SELECTED_OPTIONS := $(DEBUG_OPTIONS)
debug: $(SDW_OBJECT_FILES) $(OBJECT_FILES)
	$(COMPILER) $(LINKER_OPTIONS) $(DEBUG_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILES) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule to help find runtime errors (when you get a segmentation fault)
# NOTE: This needs the "Address Sanitizer" library to be installed in order to work (so it might not work on lab machines !)
diagnostic: override SELECTED_OPTIONS := $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS)
diagnostic: $(SDW_OBJECT_FILES) $(OBJECT_FILES)
	$(COMPILER) $(LINKER_OPTIONS) $(FUSSY_OPTIONS) $(SANITIZER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILES) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule to build for high performance executable (for manually testing interaction)
speedy: override SELECTED_OPTIONS := $(SPEEDY_OPTIONS)
speedy: $(SDW_OBJECT_FILES)
	$(COMPILER) $(LINKER_OPTIONS) $(SPEEDY_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILES) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule to compile and link for final production release
production: override SELECTED_OPTIONS :=
production: $(SDW_OBJECT_FILES)
	$(COMPILER) $(LINKER_OPTIONS) -o $(EXECUTABLE) $(OBJECT_FILES) $(SDW_LINKER_FLAGS) $(SDL_LINKER_FLAGS)
	./$(EXECUTABLE)

# Rule for building all of the the DisplayWindow classes
$(BUILD_DIR)/%.o: $(SDW_DIR)%.cpp
	@mkdir -p $(BUILD_DIR)
	$(COMPILER) $(COMPILER_OPTIONS) -c -o $@ $^ $(SDL_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

# Rule for building all of the sources
$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(shell dirname $@)
	$(COMPILER) $(COMPILER_OPTIONS) $(SELECTED_OPTIONS) -o $@ $^ $(SDL_COMPILER_FLAGS) $(SDW_COMPILER_FLAGS) $(GLM_COMPILER_FLAGS)

# Files to remove during clean
.PHONY: clean
clean:
	rm -r $(BUILD_DIR)/*