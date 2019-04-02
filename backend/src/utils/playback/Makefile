PROJ = playback
CC = g++

CFLAGS = -c -O2 -g -Wall
LDFLAGS = -L/opt/local/lib -L$(HOME)/cppunit/lib
LIBS = -lportaudio
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

# automatic documentation generation
DOC = doxygen
DOC_CONFIG = doxygen.conf
DOCS_DIR = doc

all: $(PROJ)

$(PROJ): $(OBJS)
		$(CC) $(LIBS) $^ -o $@ $(VAR)

%.o: %.cpp
		$(CC) $(CFLAGS) $< -o $@

%.o: %.cpp %.h
		$(CC) $(CFLAGS) $< -o $@

clean:
		rm -f $(PROJ) $(OBJS)

# # build HTML documentation
# doc:
# 	$(DOC) $(DOC_CONFIG)

# # build and open documentation in browser
# viewdoc: doc
# 	open $(DOCS_DIR)/index.html

# # delete all executables and object files
# # clean:
# # 	$(RM) $(OBJ)
# # 	$(RM) $(TARGET)

# # delete all executables, object files, and HTML documentation
# veryclean: clean
# 	$(RM) -r $(DOCS_DIR)