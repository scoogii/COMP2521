// Dijkstra ADT interface
// COMP2521 Assignment 2

// Note: You MUST NOT modify this file.

#include <stdbool.h>

#include "Graph.h"

typedef struct PredNode {
	Vertex v;
	struct PredNode *next;
} PredNode;

typedef struct ShortestPaths {
	int numNodes;    // The number of vertices in the graph
	
	Vertex src;      // The source vertex
	
	int *dist;       // An  array  of distances from the source vertex -
	                 // one for each vertex (the distance  from  src  to
	                 // itself is 0)
	                 // dist[v] contains the distance from src to v
	                 
	PredNode **pred; // An  array  of  predecessor  lists - one for each
	                 // vertex
	                 // pred[v] contains the predecessor list for vertex
	                 // v
} ShortestPaths;

/**
 * Finds  all  shortest  paths  from  a given source vertex to all other
 * vertices as discussed in the lectures.
 *
 * This  function  offers  one **additional feature** - if there is more
 * than one shortest path from the source vertex to  another  vertex,  a
 * vertex  could  have  multiple predecessors (see spec for an example).
 * The function must keep track of multiple predecessors for each vertex
 * (if  they  exist)  by storing the predecessor(s) in the corresponding
 * linked list for that vertex. This will be discussed in detail in  the
 * lecture.
 * 
 * For  example,  suppose that while finding the shortest paths from the
 * source vertex 0 to all other vertices, we discovered  that  vertex  1
 * has two possible predecessors on different shortest paths. 
 * 
 * Node 0
 *   Distance
 *     0 : X
 *     1 : 2
 *     2 : 1
 *   Preds
 *     0 : NULL
 *     1 : [0]->[2]->NULL
 *     2 : [0]->NULL
 * 
 * Node 1
 *   Distance
 *     0 : 2
 *     1 : X
 *     2 : 3
 *   Preds
 *     0 : [1]->NULL
 *     1 : NULL
 *     2 : [0]->NULL
 * 
 * Node 2
 *   Distance
 *     0 : 3
 *     1 : 1
 *     2 : X
 *   Preds
 *     0 : [1]->NULL
 *     1 : [2]->NULL
 *     2 : NULL
 * 
 * The  function  returns  a 'ShortestPaths' structure with the required
 * information:
 * - the number of vertices in the graph
 * - the source vertex
 * - distance array
 * - array of predecessor lists
 */
ShortestPaths dijkstra(Graph g, Vertex src);

/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We  will  not call this function during testing, so you may print out
 * the given ShortestPaths structure in whatever format you want.
 */
void showShortestPaths(ShortestPaths sps);

/**
 * Frees all memory associated with the given ShortestPaths structure.
 */
void freeShortestPaths(ShortestPaths sps);

