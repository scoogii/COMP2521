////////////////////////////////////////////////////////////////////////
//                      COMP2521 Assignment 2                         //
//                     Social Network Analysis                        //
//                            Dijkstra                                //
//                       Implementation File                          //
//                                                                    //
//                   Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

////////////////////////////////////////////////////////////////////////
//  					 PART 1 FUNCTIONS                             //
////////////////////////////////////////////////////////////////////////


/**
 * Finds  all  shortest  paths  from  a given source vertex to all other
 * vertices
 *
 * If there is more than than one shortest path from the source vertex
 * to  another  vertex,  a vertex can have  multiple predecessors
 * The function keeps track of multiple predecessors for each vertex
 * (if  they  exist)  by storing the predecessor(s) in the corresponding
 * linked list for that vertex.
 */
ShortestPaths dijkstra(Graph g, Vertex src) {
    // Create a new shortest path struct and initialise data
    ShortestPaths sps;
    sps.numNodes = GraphNumVertices(g);
    sps.src = src;
    
    // Initialisation sourced from `dijkstraSSSP` lecture code
    // Initalise all distances to INT_MAX 
    for (int i = 0; i < sps.numNodes; i++) sps.dist[i] = INT_MAX;
    sps.dist[src] = 0;

    // Initialise all predecessor nodes of vertices to NULL (i.e. no parent found)
    for (int i = 0; i < sps.numNodes; i++) sps.pred[i] = NULL;

    // Create a 'vSet' via a PQueue
    PQ vSet = PQnew();


    return sps;
}


/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We  will  not call this function during testing, so you may print out
 * the given ShortestPaths structure in whatever format you want.
 */
void showShortestPaths(ShortestPaths sps) {
    printf("Node %d\n", sps.src);

    // Printing distances of src to vertices
    printf("\tDistance\n");
    for (int i = 0; i < sps.numNodes; i++) {
        if (i == sps.src) {
            printf("\t\t%d: X\n", sps.src);
        } else {
            printf("\t\t%d: %d\n", i, sps.dist[i]);
        }
    }

    // Printing the predecessor(s) of each vertex in the graph
    printf("\tPreds\n");
    struct PredNode **current = sps.pred;
    for (int i = 0; i < sps.numNodes; i++) {
        printf("\t\t%d : ", i);
        for (; current[i] != NULL; current[i] = current[i]->next) {
            printf("[%d]->", current[i]->v);
        }
        printf("NULL\n");
    }
}


/**
 * Frees all memory associated with the given ShortestPaths structure.
 */
void freeShortestPaths(ShortestPaths sps) {}
