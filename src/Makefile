APP = Aliens
BIN = ../bin
INCLUDE = ../include
LIB = ../lib
SRC = $(APP).c
LIB_UT = util/libutil.a
LIB_GRAPH = graphics/libgraphics.a
#LIB_SEM = semaphore/libsemaphore.a

CC = gcc
CFLAGS = -Wall -lm `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf

LIBS = $(LIB)/$(LIB_UT) $(LIB)/$(LIB_GRAPH) 


ALL: APP 
 
APP: 
		$(CC) -o $(BIN)/$(APP) $(SRC) $(LIBS) -I $(INCLUDE) $(CFLAGS)