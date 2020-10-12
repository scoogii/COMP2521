# COMP2521 Lab 05 Makefile

CC = gcc
CFLAGS = -Wall -Werror -Wno-unused-variable -Wno-unused-function -std=c11 -g

all: runway testTree

runway: runway.c Schedule.c Tree.c Time.c

testTree: testTree.c Tree.c Time.c

.PHONY: clean
clean:
	rm -f runway testTree

