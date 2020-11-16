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
 * Given src vertex's predecessor list for dest, finds the total number of
 * shortest paths from vertex src to dest and returns number
 */
static int findNumSPS(PredNode *destPred) {
    int numSPS = 0;
    // A shortest path is counted whenever there exists a predecessor node for dest
    for (PredNode *current = destPred; current != NULL; current = current->next) {
        numSPS++;
    }

    return numSPS;
}


/*
 * Checks whether a vertex is in between two vertices in a shortest path route
 * Returns true if vertex is indeed betwee, otherwise false
 */
static bool isBetween(ShortestPaths srcSPS, ShortestPaths bwSPS, Vertex bw, Vertex dest) {
    if (srcSPS.dist[bw] != 0 && bwSPS.dist[dest] != 0) return true;

    return false;
}


/*
 * Finds the number of appearances of vertex bw in shortest path from src to dest
 * Backtracks from dest to src and counts when vertex bw appears
 */
static int countAppearances(ShortestPaths srcSPS, Vertex bw, Vertex dest) {
    int numAppearances = 0;
    // Loop and backtrack - increment whenever vertex bw appears with ***RECURSION*** :D
    for (PredNode *current = srcSPS.pred[dest]; current != NULL; current = current->next) {
        if (current->v == bw) return 1;
        numAppearances += countAppearances(srcSPS, bw, current->v);
    }

    return numAppearances;
}


/*
 * Calculates the number of times the vertex v is between vertex src and dest
 * in the shortest path of src to dest
 */
static int findNumBetween(ShortestPaths srcSPS, ShortestPaths bwSPS, Vertex src, Vertex bw, Vertex dest) {
    if (isBetween(srcSPS, bwSPS, bw, dest)) {
        return countAppearances(srcSPS, bw, dest);
    }

    return 0;
}


static double calculateBC(int numerator, int denominator) {
    return (double)(numerator)/denominator;
}


/**
 * Finds the betweenness centrality for each vertex in the given graph
 * and returns the results in a NodeValues structure.
 * Really bad time complexity I know SORRY
 */
NodeValues betweennessCentrality(Graph g) {
    int numVertices = GraphNumVertices(g);
    // Create new NodeValues struct to store betweenness centrality data
    NodeValues bcData = newNVS(numVertices);

    for (Vertex bw = 0; bw < numVertices; bw++) {
        ShortestPaths bwSPS = dijkstra(g, bw);

        for (Vertex src = 0; src < numVertices; src++) {
            ShortestPaths srcSPS = dijkstra(g, src);
            
            for (Vertex dest = 0; dest < numVertices; dest++) {
                if (src != bw && bw != dest && src != dest) {
                    int numSPS = findNumSPS(srcSPS.pred[dest]);
                    if (numSPS != 0) {
                        int numBetween = findNumBetween(srcSPS, bwSPS, src, bw, dest);
                        bcData.values[bw] += calculateBC(numBetween, numSPS); 
                    }
                }
            }
            freeShortestPaths(srcSPS);
        }
        freeShortestPaths(bwSPS);
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
