// Centrality Measures ADT interface
// COMP2521 Assignment 2

// Note: You MUST NOT modify this file.

#ifndef CENTRALITY_MEASURES_H
#define CENTRALITY_MEASURES_H

#include <stdbool.h>

#include "Graph.h"

typedef struct NodeValues {
	int numNodes;   // The number of nodes in the graph
	double *values; // An  array  of  values, one  for  each vertex. The
	                // meaning of the values depends on  which  function
	                // is being called.
} NodeValues;


/**
 * Finds the closeness centrality for each vertex in the given graph and
 * returns the results in a NodeValues structure.
 */
NodeValues closenessCentrality(Graph g);

/**
 * Finds  the  betweenness centrality for each vertex in the given graph
 * and returns the results in a NodeValues structure.
 */
NodeValues betweennessCentrality(Graph g);

/**
 * Finds  the  normalised  betweenness centrality for each vertex in the
 * given graph and returns the results in a NodeValues structure.
 */
NodeValues betweennessCentralityNormalised(Graph g);

/**
 * Prints  the values in the given NodeValues structure to stdout in the
 * format:
 * 0: <value for node 0>
 * 1: <value for node 1>
 * ...
 */
void showNodeValues(NodeValues nvs);

/**
 * Frees all memory associated with the given NodeValues structure.
 */
void freeNodeValues(NodeValues nvs);

#endif

