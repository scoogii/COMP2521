////////////////////////////////////////////////////////////////////////
//                      COMP2521 Assignment 2                         //
//                     Social Network Analysis                        //
//                     Lance-Williams Algorithm                       //
//                       Implementation File                          //
//                                                                    //
//                   Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "LanceWilliamsHAC.h"

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                         PART 2 HELPERS                             //
//                                                                    //
////////////////////////////////////////////////////////////////////////

/*
 * Finds the lenght of an adjacency list and returns the value
 */
static int listLength(AdjList l) {
    int listLength = 0;
    for (AdjList current = l; current != NULL; current = current->next) {
        listLength++;
    }
    
    return listLength;
}


/*
 * Finds the passed in vertex's weight if it exists in the given
 * adjacency list of another vertex
 */
static int findWeight(AdjList l, Vertex v) {
    for (AdjList current = l; current != NULL; current = current->next) {
        if (current->v == v) return current->weight;
    }

    return INT_MAX;
}


/*
 * Compares two weights and returns the max of the two
 * Considers if w2 does not exist since function that calls max
 * uses GraphOutIncident and GraphInIncident from 'v'
 */
static int max(int w1, int w2) {
    if (w2 == INT_MAX) {
        return w1;
    }

    return (w1 > w2) ? w1 : w2;
}


/*
 * Updates the distance array after merging clusters
 */
static void updateDist(Graph g, int **dist, int numV) {
    for (Vertex v = 0; v < numV; v++) {
        AdjList vOutEdges = GraphOutIncident(g, v);
        AdjList vInEdges = GraphInIncident(g, v);
        for (Vertex w = 0; w < listLength(vOutEdges); w++) {
            int vOut = findWeight(vOutEdges, w);  // Edge v -> w
            int wOut = findWeight(vInEdges, v);  // Edge w -> v 
            int distVW = max(vOut, wOut);
            dist[v][w] = (distVW == INT_MAX) ? INT_MAX : (double)1/distVW;
        }
    }
}


////////////////////////////////////////////////////////////////////////
//                                                                    //
//                        PART 2 FUNCTIONS                            //
//                                                                    //
////////////////////////////////////////////////////////////////////////

/**
 * Generates  a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int numVertices = GraphNumVertices(g);
    int **dist = malloc((numVertices * numVertices) * sizeof(int));

    updateDist(g, dist, numVertices);

    Dendrogram fag = NULL;
    return fag;
}


/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) {
    if (d != NULL) {
        freeDendrogram(d->left);
        freeDendrogram(d->right);
        free(d);
    }
}
