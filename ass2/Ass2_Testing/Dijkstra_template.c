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
 * Creates a new predecessor node and returns it
 */
static PredNode *newPredNode(Vertex v) {
    PredNode *new = malloc(sizeof(struct PredNode));
    new->v = v;
    new->next = NULL;

    return new;
}


/**
 * Inserts a new predecessor node to a given predecessor list
 */
static PredNode *PredNodeInsert(PredNode *pred, PredNode *newNode) {
    // If the predecessor list is empty, 
    if (pred == NULL) {
        pred = newNode;
        return pred;
    }
        
    // Otherwise, insert as tail
    PredNode *current = pred;
    while (current->next != NULL) current = current->next;
    current->next = pred;

    return pred;
}


/**
 * Edge relaxation function that updates the data of dist array if a shorter
 * path is found. For example, if dist[v] + weight < dist[w] then we update
 * the distance array such that dist[w] <-- dist[v] + weight and pred[w] <--v
 * where dist[v] is the length of shortest known path from src to v
 * and dist[w] is the length of shortest known path from src to w
 */
static ShortestPaths relaxEdge(ShortestPaths sps, int v, int w, int weight) {
    if (v + weight < w) {
        sps.dist[w] = v + weight;
        sps.pred[w] = PredNodeInsert(sps.pred[w], newPredNode(v));
    }

    return sps;
}


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
    for (int i = 0; i < sps.numNodes - 1; i++) sps.dist[i] = INT_MAX;
    sps.dist[src] = 0;

    // Initialise all predecessor nodes of vertices to NULL (i.e. no parent found)
    for (int i = 0; i < sps.numNodes - 1; i++) sps.pred[i] = NULL;

    // Create a 'vertice set' via a PQueue - src should start at highest priority
    PQ vSet = PQNew();
    for (int i = 0; i < sps.numNodes - 1; i++) PQInsert(vSet, i, 1);
    PQUpdate(vSet, src, 0);

    while (!PQIsEmpty(vSet)) {
        int v = PQDequeue(vSet);
    
        // Get all adjacent vertices from current vertex v
        AdjList outLinks = GraphOutIncident(g, v);
        for (; outLinks != NULL; outLinks = outLinks->next) {
            sps = relaxEdge(sps, sps.dist[v], sps.dist[outLinks->v], outLinks->weight);
        }
    }


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
void freeShortestPaths(ShortestPaths sps) {
    // Loop through all predecessor lists and free one node at a time
    for (int i = 0; i < sps.numNodes; i++) {
        struct PredNode *current = sps.pred[i];
        sps.pred[i] = sps.pred[i]->next;
        free(current);
    }
}
