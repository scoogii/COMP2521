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

////////////////////////////////////////////////////////////////////////
 //                          AUXILLARY                                //
////////////////////////////////////////////////////////////////////////

/*
 * Finds the weight of an edge given an adjacency list and a vertex
 * If the given AdjList has vertices, then the correct weight is returned
 * Otherwise, if there are no edges, return INT_MAX (i.e. infinity)
 */
static int findWeight(AdjList l, Vertex v) {
    // If AdjList NULL, then no outgoing edges exist, return infinity
    if (l == NULL) return INT_MAX;

    for (AdjList current = l; current != NULL; current = current->next) {
        if (l->v == v) break;
    }

    return l->weight;
}


/*
 * Returns the max between two edges
 * Takes infinity edges into account
 */
static double max(int w1, int w2) {
    // If one edge doesn't exist, return other edge
    if (w1 == INT_MAX) return w2;
    if (w2 == INT_MAX) return w1;

    // Otherwise, return the larger weight
    return (w1 > w2) ? w1 : w2;
}


/*
 * Sets all cells of a newly created distance 2-D array to infinity
 */
static void setInfinity(int numV, double dist[numV][numV]) {
    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            dist[i][j] = INT_MAX;
        }
    }
}

////////////////////////////////////////////////////////////////////////
//                               DIST                                 //
////////////////////////////////////////////////////////////////////////

/*
 * Updates the distance array after merging clusters
 */
static void initialiseDist(Graph g, int numV, double dist[numV][numV]) {
    for (Vertex v = 0; v < numV; v++) {
        AdjList vOutEdges = GraphOutIncident(g, v);
        for (AdjList current = vOutEdges; current != NULL; current = current->next) {
            int w = current->v;
            int vOut = findWeight(vOutEdges, w);  // edge weight v->w
            dist[v][w] = 1/max(vOut, dist[v][w]);
            dist[w][v] = dist[v][w];
        }
    }
}

////////////////////////////////////////////////////////////////////////
//                           DENDROGRAMZ                              //
////////////////////////////////////////////////////////////////////////

/*
 * Initialises a new dendrogram node and returns it
 */
static Dendrogram newDend(int v) {
    Dendrogram new = malloc(sizeof(DNode));
    new->vertex = v;
    new->left = NULL;
    new->right = NULL;

    return new;
}


/*
 * Initialises an array where each cell is a pointer to a dendrogram
 * that stores a single vertex in it
 */
static void initialiseDend(int numV, Dendrogram dendA[numV]) {
    for (int i = 0; i < numV; i++) dendA[i] = newDend(i);
}


/*
 * Finds the two closest clusters and returns which clusters are the closest
 * Stores the clusters in separate variables
 */
static void findClosestClusters(int numV, double dist[numV][numV], int *c1, int *c2) {
    int currentMin = INT_MAX;
    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            if (dist[i][j] < currentMin) {
                *c1 = i;
                *c2 = j;
            }
        }
    }
}


/*
 * Merges the two closest clusters together and adds it to a smaller
 * dendrogram array
 */
static Dendrogram mergeClusters(int numV, double dist[numV][numV], Dendrogram dendA[numV]) {
    int c1; int c2;
    findClosestClusters(numV, dist, &c1, &c2);
    int currentMin = INT_MAX;

}

/*
 * Generates a final dendrogram by incrementally reducing the size of
 * the dendA array and dist 2-D array
 */ 
static Dendrogram makeFinalDend(int numV, double dist[numV][numV], Dendrogram dendA[numV]) {
    int i = 1; int j = 1;
    for (int i = 0; i < numV; i++) {
        mergeClusters(numV, dist, dendA);
    }

}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                        PART 2 FUNCTIONS                            //
//                                                                    //
////////////////////////////////////////////////////////////////////////

/**
 * Generates a Dendrogram using the Lance-Williams algorithm (discussed
 * in the spec) for the given graph  g  and  the  specified  method  for
 * agglomerative  clustering. The method can be either SINGLE_LINKAGE or
 * COMPLETE_LINKAGE (you only need to implement these two methods).
 * 
 * The function returns a 'Dendrogram' structure.
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int numVertices = GraphNumVertices(g);
    // Initialise a dist array to be used later to create a dendrogram
    double dist[numVertices][numVertices];
    setInfinity(numVertices, dist);

    // Calculate distances for vertices in the dist array using LW method
    // and initialise an array of dendrograms
    initialiseDist(g, numVertices, dist);

    Dendrogram dendA[numVertices];
    initialiseDend(numVertices, dendA);

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            printf("\t%lf\t", dist[i][j]);
        }
        printf("\n");
    }

    // Call function that returns the final completed dendrogram
    /*Dendrogram final = makeFinalDend(numVertices, dist, dendA);*/

    Dendrogram final = dendA[0];
    return final;
}


/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) {
    // Yeah... totally didn't get this from BSTree.h
    if (d != NULL) {
        freeDendrogram(d->left);
        freeDendrogram(d->right);
        free(d);
    }
}
