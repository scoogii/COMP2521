/*
 * testCentralityMeasures.c
 * Written By Zain Afzal for cs2521 
 *
 * Includes basic interface to read a graph in from
 * a file and a measure and calls the related functions
 *
 * Feel free to override this with your own tests if
 * you so wish.
 */
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Graph.h"
#include "GraphRead.h"

#define BUFF_SIZE 1024

static void printUsage(void);

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printUsage();
		return EXIT_FAILURE;
	}
	
	NodeValues (*fn)(Graph) = NULL;
	
	Graph g = readGraph(argv[1]);

	if (argv[2][0] == 'c' && argv[2][1] == '\0') {
		fn = closenessCentrality;
	} else if (argv[2][0] == 'b' && argv[2][1] == '\0') {
		fn = betweennessCentrality;
	} else if (argv[2][0] == 'b' && argv[2][1] == 'n' ) {
		fn = betweennessCentralityNormalised;
	} else {
		printUsage();
	}
	
	if (fn != NULL) {
		NodeValues val = fn(g);
		showNodeValues(val);
		freeNodeValues(val);
	}
	
	GraphFree(g);
	

}

static void printUsage(void) {
	printf("Usage: ./testCentralityMeasures [file] [flag]\n");
	printf("Valid Flags:\n");
	printf("    c    : closeness centrality\n");
	printf("    b    : betweenness centrality\n");
	printf("    bn   : betweenness centrality normalised\n");
}

