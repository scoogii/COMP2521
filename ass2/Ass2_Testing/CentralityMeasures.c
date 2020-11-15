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
static double calcClosenessCentrality(int sum, int n, int N) {
    return (double)(n - 1)/(N-1) * (n - 1)/sum;
}


/*
 * Determines whether a vertex is reachable or not
 * Returns true if reachable, otherwise false
 */
static bool isReachable(Graph g, Vertex v) {
    if (GraphInIncident(g, v) == NULL && GraphOutIncident(g, v) == NULL) return false;

    return true;
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
    NodeValues ccData;
    ccData.numNodes = numVertices;
    ccData.values = malloc(numVertices * sizeof(double));

    // Loop through all vertices to calculate closeness centrality
    for (Vertex i = 0; i < GraphNumVertices(g); i++) {
        ShortestPaths sps = dijkstra(g, i);
        int distSum = sumDistances(sps.dist, numVertices);
        int n = numReachableVertices(sps.dist, numVertices) + 1;
        int N = numVertices;

        // If the vertex is unreachable, it's cc is 0, otherwise calculate
        // Also, if there is only one node in entire graph, also set cc to 0
        if (!isReachable(g, i) || distSum == 0) {
            ccData.values[i] = 0;
        } else {
            ccData.values[i] = calcClosenessCentrality(distSum, n, N);
        }

        freeShortestPaths(sps);
    }

    return ccData;
}
