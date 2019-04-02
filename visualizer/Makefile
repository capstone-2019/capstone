PROJ = produce_cso

CC = g++
LIBS = -lsndfile
OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

produce_cso: produce_cso.o
	$(CC) $(LIBS) -o produce_cso produce_cso.o

clean:
	rm -f $(PROJ) $(OBJS)