# name of the executable to produce
TARGET = csim

# the C++ compiler to use
CC = g++

SRC_DIRS = src   # where source files are located
INC_DIRS = inc   # header files to add to include path

# generate the set of object files to produce
SRC = $(shell find $(SRC_DIRS) -name *.cpp)
OBJ = $(SRC:%.cpp=%.o)

# compiler/linker flags
INC_FLAGS = $(addprefix -I, $(INC_DIRS))
STANDARD_FLAGS = -std=c++17
CPP_FLAGS = $(INC_FLAGS) $(STANDARD_FLAGS)

# automatic documentation generation
DOC = doxygen
DOC_CONFIG = doxygen.conf
DOCS_DIR = doc

# build the top-level executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

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

# delete all executables, object files, and HTML documentation
veryclean: clean
	$(RM) -r $(DOCS_DIR)

