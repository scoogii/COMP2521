# Makefile for Ass2

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -g
AR = ar
objs = Graph.o CentralityMeasures.o PQ.o Dijkstra.o GraphRead.o 

all: testDijkstra testCentralityMeasures testLanceWilliamsHAC

$(objs): %.o : %.c

GraphLib.a: $(objs)
	$(AR) rcs $@ $^

testDijkstra: testDijkstra.c GraphLib.a
	$(CC) $(CFLAGS) -o testDijkstra testDijkstra.c GraphLib.a -lm

testCentralityMeasures: testCentralityMeasures.c GraphLib.a
	$(CC) $(CFLAGS) -o testCentralityMeasures testCentralityMeasures.c GraphLib.a -lm

BSTree.o: BSTree.c BSTree.h 
	$(CC) $(CFLAGS) -c BSTree.c  

LanceWilliamsHAC.o: LanceWilliamsHAC.c Graph.o BSTree.o 
	$(CC) $(CFLAGS) -c -o LanceWilliamsHAC.o LanceWilliamsHAC.c -lm

testLanceWilliamsHAC: testLanceWilliamsHAC.c Graph.h Graph.o LanceWilliamsHAC.o BSTree.o GraphRead.o
	$(CC) $(CFLAGS) -o testLanceWilliamsHAC testLanceWilliamsHAC.c Graph.o BSTree.o GraphRead.o LanceWilliamsHAC.o -lm

clean:
	rm -f *.o testCentralityMeasures testDijkstra testLanceWilliamsHAC GraphLib.a

