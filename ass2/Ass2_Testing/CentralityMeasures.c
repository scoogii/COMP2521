////////////////////////////////////////////////////////////////////////
//                      COMP2521 Assignment 2                         //
//                     Social Network Analysis                        //
//                       Centrality Measures                          //
//                       Implementation File                          //
//                                                                    //
//                   Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"

////////////////////////////////////////////////////////////////////////

static int findNumPaths(PredNode **pred, Vertex src, Vertex dest);
static int countAppearances(PredNode **pred, Vertex src, Vertex v, Vertex dest);


////////////////////////////////////////////////////////////////////////
//                         PART 2 HELPERS                             //
////////////////////////////////////////////////////////////////////////


/*
 * Sums the total distance of all reachable vertices from
 * a src vertex and returns the value
 */
static int sumDistances(int *dist, int numVertices) {
    int sum = 0;
    for (int i = 0; i < numVertices; i++) {
        sum += dist[i];
    }

    return sum;
}


/*
 * Counts the number of reachable vertices and returns it.
 * A vertex is considered unreachable if the distance is 0
 */
static int numReachableVertices(int *dist, int numVertices) {
    int numReachable = 0;
    for (int i = 0; i < numVertices; i++) {
        if (dist[i] != 0) numReachable++; 
    }

    return numReachable;
}


/*
 * Calculates the closeness centrality of a vertex using the
 * Wasserman and Faust formula
 */
static double calculateCC(int sum, int n, int N) {
    return (double)(n - 1)/(N-1) * (n - 1)/sum;
}


/*
 * Determines whether a vertex is isolated or not
 * Returns true if reachable, otherwise false
 */
static bool isIsolated(Graph g, Vertex v) {
    if (GraphInIncident(g, v) == NULL && GraphOutIncident(g, v) == NULL)
        return false;

    return true;
}


/*
 * Creates a new NodeValues struct and returns it
 */
static NodeValues newNVS(int numVertices) {
    NodeValues new;
    new.numNodes = numVertices;
    new.values = calloc(numVertices, sizeof(double));

    return new;
}


////////////////////////////////////////////////////////////////////////
//                        PART 2 FUNCTIONS                            //
////////////////////////////////////////////////////////////////////////


/**
 * Prints  the values in the given NodeValues structure to stdout in the
 * format:
 * 0: <value for node 0>
 * 1: <value for node 1>
 * ...
 */
void showNodeValues(NodeValues nvs) {
    for (int i = 0; i < nvs.numNodes; i++) {
        printf("%d: %lf\n", i, nvs.values[i]);
    }
}


/**
 * Frees all memory associated with the given NodeValues structure.
 */
void freeNodeValues(NodeValues nvs) {
    free(nvs.values);
}


/**
 * Finds the closeness centrality for each vertex in the given graph and
 * returns the results in a NodeValues structure.
 */
NodeValues closenessCentrality(Graph g) {
    int numVertices = GraphNumVertices(g);
    // Create new NodeValues struct to store closeness centrality data
    NodeValues ccData = newNVS(numVertices);

    // Loop through all vertices to calculate closeness centrality
    for (Vertex i = 0; i < numVertices; i++) {
        ShortestPaths sps = dijkstra(g, i);
        int distSum = sumDistances(sps.dist, numVertices);
        int n = numReachableVertices(sps.dist, numVertices) + 1;

        // If vertex is isolated or sum of shortest paths is 0, it's cc is 0
        if (!isIsolated(g, i) || distSum == 0) {
            ccData.values[i] = 0;
        } else {
            ccData.values[i] = calculateCC(distSum, n, numVertices);
        }

        freeShortestPaths(sps);
    }

    return ccData;
}


/*
 * Finds the numbers of shortest paths from src to dest and returns it
 */
static int findNumPaths(PredNode **pred, Vertex src, Vertex dest) {
    if (src == dest) return 1;

    int numPaths = 0;
    for (PredNode *current = pred[dest]; current != NULL; current = current->next) {
        numPaths += findNumPaths(pred, src, current->v);
    }

    return numPaths;
}


/*
 * Finds the number of appearances of vertex v in shortest path from src to dest
 * Backtracks from dest to src and counts when vertex v appears
 */
static int countAppearances(PredNode **pred, Vertex src, Vertex v, Vertex dest) {
    if (v == dest) return findNumPaths(pred, src, v);

    // Loop and backtrack - increment whenever vertex bw appears
    int numAppearances = 0;
    for (PredNode *current = pred[dest]; current != NULL; current = current->next) {
        numAppearances += countAppearances(pred, src, v, current->v);
    }

    return numAppearances;
}


/*
 * Calculates betweenness centrality using the provided formula
 */
static double calculateBC(PredNode **pred, Vertex src, Vertex bw, Vertex dest) {
    int numSPS = findNumPaths(pred, src, dest);
    if (numSPS == 0) return 0;

    int numBW = countAppearances(pred, src, bw, dest);

    return (double)(numBW)/numSPS;
}


/**
 * Finds the betweenness centrality for each vertex in the given graph
 * and returns the results in a NodeValues structure.
 */
NodeValues betweennessCentrality(Graph g) {
    int numVertices = GraphNumVertices(g);
    // Create new NodeValues struct to store betweenness centrality data
    NodeValues bcData = newNVS(numVertices);

    for (Vertex bw = 0; bw < numVertices; bw++) {  // Loop through each 'between' vertex
        for (Vertex src = 0; src < numVertices; src++) {  // Loop through each 'source' vertex
            ShortestPaths srcSPS = dijkstra(g, src);
            for (Vertex dest = 0; dest < numVertices; dest++) {  // Loop through each 'dest' vertex
                if (src != bw && bw != dest && src != dest) {
                    bcData.values[bw] += calculateBC(srcSPS.pred, src, bw, dest); 
                }
            }
            freeShortestPaths(srcSPS);
        }
    }    

    return bcData;
}


/*
 * Helper function that normalises the betweenness centrality of a vertex
 */
static double normaliseBC(double bc, int n) {
    if (n < 2) return 0;

    return (double)1/((n - 1) * (n - 2)) * bc;
}


/**
 * Finds  the  normalised  betweenness centrality for each vertex in the
 * given graph and returns the results in a NodeValues structure.
 */
NodeValues betweennessCentralityNormalised(Graph g) {
    int numVertices = GraphNumVertices(g);
    // Create new NodeValues struct that contains bc values to be normalise
    NodeValues bnData = betweennessCentrality(g);

    for (Vertex i = 0; i < numVertices; i++) {
        bnData.values[i] = normaliseBC(bnData.values[i], numVertices);
    }

    return bnData;
}
