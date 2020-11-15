////////////////////////////////////////////////////////////////////////
//                      COMP2521 Assignment 2                         //
//                     Social Network Analysis                        //
//                       Centrality Measures                          //
//                       Implementation File                          //
//                                                                    //
//                   Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "Graph.h"

////////////////////////////////////////////////////////////////////////
//                         PART 2 HELPERS                             //
////////////////////////////////////////////////////////////////////////

static int sumDistances(int *dist, int numVertices) {
    int sum = 0;
    for (int i = 0; i < numVertices; i++) {
        sum += dist[i];
    }

    return sum;
}


static int numReachableVertices(int *dist, int numVertices) {
    int numReachable = 0;
    for (int i = 0; i < numVertices; i++) {
        if (dist[i] != 0) numReachable++; 
    }

    return numReachable;
}


static double calcClosenessCentrality(int sum, int n, int N) {

    return 0;
}


////////////////////////////////////////////////////////////////////////
//                        PART 2 FUNCTIONS                            //
////////////////////////////////////////////////////////////////////////


/**
 * Finds the closeness centrality for each vertex in the given graph and
 * returns the results in a NodeValues structure.
 */
NodeValues closenessCentrality(Graph g) {
    // Create new NodeValues struct to store closeness centrality data
    NodeValues closeCentralityData;
    closeCentralityData.numNodes = GraphNumVertices(g);

    // Loop through all vertices and calculate their closeness centrality
    // Use Wasserman and Faust Formula to calculate
    for (Vertex i = 0; i < GraphNumVertices(g); i++) {
        ShortestPaths sps = dijkstra(g, i);
        int distanceSum = sumDistances(sps.dist, GraphNumVertices(g));
        int n = numReachableVertices(sps.dist, GraphNumVertices(g));
        int N = sps.numNodes;

        closeCentralityData.values[i] = calcClosenessCentrality(distanceSum, n, N);

        freeShortestPaths(sps);
    }

    return closeCentralityData;
}
