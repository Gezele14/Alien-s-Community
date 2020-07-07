APP = Aliens
BIN = ../bin
INCLUDE = ../include
LIB = ../lib
SRC = $(APP).c
#LIB_UT = util/libutil.a
#LIB_SM = shareMem/libshareMem.a
#LIB_SEM = semaphore/libsemaphore.a

CC = gcc
CFLAGS = -Wall -lm `sdl2-config --cflags --libs`

#LIBS = $(LIB)/$(LIB_UT) $(LIB)/$(LIB_SM) $(LIB)/$(LIB_SEM)


ALL: APP 
 
APP: 
		$(CC) -o $(BIN)/$(APP) $(SRC)  $(CFLAGS)