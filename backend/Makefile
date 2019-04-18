# name of the executable to produce
TARGET = csim

# the C++ compiler to use
CC = g++

SRC_DIRS = src
INC_DIRS = inc/sim inc/sim/eigen inc/audio_processor inc/effects
EXCLUDE_DIRS = src/utils

UTILS_DIRS = src/utils
UTILS_SRC = $(shell find $(UTILS_DIRS) -name *.cpp)
UTILS_OBJS = $(UTILS_SRC:%.cpp=%.o)

# generate the set of object files to produce
EXCLUDES = $(shell find $(EXCLUDE_DIRS) -name *.cpp)
SRC = $(filter-out $(EXCLUDES), $(shell find $(SRC_DIRS) -name *.cpp))
OBJ = $(SRC:%.cpp=%.o)

# test files
SIM_FILE = src/sim/sim.cpp
TEST_SRC = $(filter-out $(SIM_FILE), $(SRC)) test/hw_input.cpp
TEST_OBJ = $(TEST_SRC:%.cpp=%.o)

# compiler/linker flags
INC_FLAGS = $(addprefix -I, $(INC_DIRS))
STANDARD_FLAGS = -std=c++17
CPP_FLAGS = $(INC_FLAGS) $(STANDARD_FLAGS) -O3
LDFLAGS = -lsndfile -lportaudio

# automatic documentation generation
DOC = doxygen
DOC_CONFIG = doxygen.conf
DOCS_DIR = doc

# build the top-level executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

test_hw: $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $@ $(LDFLAGS)

utils:
	make -C src/utils/playback/ all
all:
	make utils
	make test_hw
	make csim

# build object file for each source file
%.o: %.cpp
	$(CC) $(CPP_FLAGS) -c $< -o $@

# build HTML documentation
doc:
	$(DOC) $(DOC_CONFIG)

# build and open documentation in browser
viewdoc: doc
	open $(DOCS_DIR)/index.html

# delete all executables and object files
clean:
	$(RM) $(OBJ)
	$(RM) $(TARGET)
	make -C src/utils/playback clean
	$(RM) $(TEST_OBJ)
	$(RM) test_hw
	$(RM) playback

# delete all executables, object files, and HTML documentation
veryclean: clean
	$(RM) -r $(DOCS_DIR)

