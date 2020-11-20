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
//                               DIST                                 //
////////////////////////////////////////////////////////////////////////

/**
 * Sets all cells of a newly created distance 2-D array to infinity
 */
static void setInfinity(int numV, double dist[numV][numV]) {
    for (int i = 0; i < numV; i++) { for (int j = 0; j < numV; j++) {
            dist[i][j] = INT_MAX;
        }
    }
}


/**
 * Initialises the distances in the 2-D dist array via the formula
 * provided by the spec
 */
static void initialiseDist(Graph g, int numV, double dist[numV][numV]) {
    for (Vertex v = 0; v < numV; v++) {
        AdjList vOutEdges = GraphOutIncident(g, v);
        for (AdjList current = vOutEdges; current != NULL; current = current->next) {
            int w = current->v;
            int vOut = current->weight;  // edge v->w
            dist[v][w] = ((double)1/vOut < dist[v][w]) ? (double)1/vOut : dist[v][w];
            dist[w][v] = dist[v][w];
        }
    }
}

////////////////////////////////////////////////////////////////////////
//                           DENDROGRAMZ                              //
////////////////////////////////////////////////////////////////////////

/**
 * Initialises a new dendrogram node and returns it
 * Leaves vertex uninitialised in the case of merging
 */
static Dendrogram newDend() {
    Dendrogram new = malloc(sizeof(DNode));
    new->left = NULL;
    new->right = NULL;

    return new;
}


/**
 * Initialises an array where each cell is a pointer to a dendrogram
 * that stores a single vertex in it
 */
static void initialiseDend(int numV, Dendrogram dendA[numV]) {
    for (int i = 0; i < numV; i++) {
        dendA[i] = newDend();
        dendA[i]->vertex = i;
    }
}


/**
 * Finds the two closest clusters and returns which clusters are the closest
 * Stores the clusters in separate pointers
 */
static void getClosestClusters(int numC, double dist[numC][numC], int *c1, int *c2) {
    // Start comparing closest clusters at (0,0)
    *c1 = 0; *c2 = 0;
    for (int i = 0; i < numC; i++) {
        for (int j = 0; j < numC; j++) {
            // Set new min distance b/w clusters if new min found
            if (dist[i][j] < dist[*c1][*c2] && i != j) {
                *c1 = i;
                *c2 = j;
            }
        }
    }
}


/**
 * Inserts a dendrogram node into a another dendrogram tree structure
 * Leaves the vertex field uninitialised since excel example doesn't require
 * it to be
 */
static Dendrogram mergeClusters(Dendrogram c1, Dendrogram c2) {
    Dendrogram new = newDend();
    new->left = c1;
    new->right = c2;

    return new;
}


/**
 * Reduces dendA by making a smaller copy of the array and adding the merged 
 * cluster at the last index
 */
static void reduceDendA(int numC, Dendrogram dendA[numC], Dendrogram reducedDendA[numC - 1], int c1, int c2) {
    int i = 0;
    for (int j = 0; i < (numC - 2); i++, j++) {
        while (j == c1 || j == c2) j++;  // leave closest clusters for last index
        if (j < numC) reducedDendA[i] = dendA[j];
    }

    // Last index becomes newly merged cluster
    reducedDendA[i] = mergeClusters(dendA[c1], dendA[c2]);
}


/**
 * Updates the newly merged cluster's values in dist array
 * using the LW formula
 * Updates the last row and last column in the reduced array
 */
static void updateDist(int numC, double dist[numC][numC], double reducedDist[numC - 1][numC - 1],
                       int c1, int c2, int method) {
    for (int i = 0, rowColCount = 0; i < numC; i++, rowColCount++) {
        while (i == c1 || i == c2) i++;  // skip past cluster 1 and 2
        if (i < numC) {
            double a = dist[c1][i];  // Dist(ci, ck)
            double b = dist[c2][i];  // Dist(cj, ck)
            double c = (a - b > 0) ? (a - b) : (b - a);  // abs(a - b)
            double methodLW = (method == SINGLE_LINKAGE) ? -1 : 1;

            // LW formula for both single and complete linkage
            double newDist = ((0.5) * a) + ((0.5) * b) + ((methodLW * (0.5)) * c);

            // Update new clusters' LW distances
            reducedDist[numC - 2][rowColCount] = newDist;
            reducedDist[rowColCount][numC - 2] = newDist;
        }
    }
    // Set distance to the newly merged cluster itself to infinity
    reducedDist[numC - 2][numC - 2] = INT_MAX;
}


/**
 * Reduces the dist array by making a smaller copy with newly merged clusters
 * New dist values for merged clusters are updated and is
 * copied back into the original dist array from the temporary one
 */
static void reduceDist(int numC, double dist[numC][numC], double reducedDist[numC - 1][numC - 1], int c1, int c2, int method) {
    int i = 0;
    // Skip past clusters 1 and 2 since we want to update it at the last index
    for (int distRow = 0; i < numC; i++, distRow++) {
        while (i == c1 || i == c2) i++;
        for (int distCol = 0, j = 0; j < numC; j++, distCol++) {
            while (j == c1 || j == c2) j++;
            if (i < numC && j < numC) reducedDist[distRow][distCol] = dist[i][j];
        }
    }
    
    updateDist(numC, dist, reducedDist, c1, c2, method);
}


/**
 * Generates a final dendrogram by incrementally 'reducing' the size of
 * the dendA array and dist 2-D array
 * Uses *recursion* to progressively reduce the size of the dendA array until it
 * contains the final, completed dendrogram
 */ 
static Dendrogram makeFinalDend(int numC, double dist[numC][numC], Dendrogram dendA[numC], int method) {
    // Stopping case: when all clusters are merged hence return complete dendrogram
    if (numC == 1) return dendA[0];

    // Start comparing clusters at (0, 1) since (0, 0) invalid
    int c1; int c2;
    getClosestClusters(numC, dist, &c1, &c2);

    // Generate a smaller dendrogram with a newly merged cluster
    Dendrogram reducedDendA[numC - 1];
    reduceDendA(numC, dendA, reducedDendA, c1, c2);

    // Generate a smaller dist 2-D array with updated LW distances
    double reducedDist[numC - 1][numC - 1];
    reduceDist(numC, dist, reducedDist, c1, c2, method);

    // Recurse until size of dendrogram array reduces to 1, i.e. 1 complete cluster
    return makeFinalDend((numC - 1), reducedDist, reducedDendA, method);
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
    initialiseDist(g, numVertices, dist);

    // Initialise an array of dendrograms to store clusters
    Dendrogram dendA[numVertices];
    initialiseDend(numVertices, dendA);

    Dendrogram final = makeFinalDend(numVertices, dist, dendA, method);

    return final;
}


/**
 * Frees all memory associated with the given Dendrogram structure.
 */
void freeDendrogram(Dendrogram d) {
    // Yeah... totally didn't get this from BSTree.h :D
    if (d != NULL) {
        freeDendrogram(d->left);
        freeDendrogram(d->right);
        free(d);
    }
}
