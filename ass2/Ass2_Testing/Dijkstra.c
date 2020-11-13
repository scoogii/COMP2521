////////////////////////////////////////////////////////////////////////
//                      COMP2521 Assignment 2                         //
//                     Social Network Analysis                        //
//                            Dijkstra                                //
//                       Implementation File                          //
//                                                                    //
//                   Christian Nguyen - z5310911                      //
////////////////////////////////////////////////////////////////////////

#include <assert.h>
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
    current->next = newNode;

    return pred;
}


/**
 * Edge relaxation function that updates the data of dist array if a shorter
 * path is found. For example, if dist[v] + weight < dist[w] then we update
 * the distance array such that dist[w] <-- dist[v] + weight and pred[w] <--v
 * where dist[v] is the length of shortest known path from src to v
 * and dist[w] is the length of shortest known path from src to w
 */
static ShortestPaths relaxEdge(ShortestPaths sps, Vertex v, Vertex w, int weight) {
    // If a shorter path is found, make that the new shortest path and free the old predecessor list
    if (sps.dist[v] + weight < sps.dist[w]) {
        sps.dist[w] = sps.dist[v] + weight;

        /*for (; sps.pred[w] != NULL; sps.pred[w] = sps.pred[w]->next) {*/
            /*struct PredNode *current = sps.pred[w];*/
            /*free(current);*/
        /*}*/

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
    sps.dist = malloc(sps.numNodes * sizeof(int));
    sps.pred = malloc(sps.numNodes * sizeof(PredNode *));

    // Initialisation sourced from `dijkstraSSSP` lecture code
    for (Vertex i = 0; i < sps.numNodes; i++) sps.dist[i] = INT_MAX;
    sps.dist[src] = 0;

    // Initialise all predecessor nodes of vertices to NULL (i.e. no parent found)
    for (Vertex i = 0; i < sps.numNodes; i++) sps.pred[i] = NULL;

    // Create a 'vertice set' via a PQueue - src should start at highest priority
    PQ vSet = PQNew();
    for (Vertex i = 0; i < sps.numNodes; i++) PQInsert(vSet, i, 1);
    PQUpdate(vSet, src, 0);

    while (!PQIsEmpty(vSet)) {
        Vertex v = PQDequeue(vSet);
    
        // Get all adjacent vertices from current vertex v and do edge relaxation
        AdjList outLinks = GraphOutIncident(g, v);
        for (; outLinks != NULL; outLinks = outLinks->next) {
            sps = relaxEdge(sps, v, outLinks->v, outLinks->weight);
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
    printf("  Distance\n");
    for (int i = 0; i < sps.numNodes; i++) {
        if (i == sps.src) {
            printf("    %d: X\n", sps.src);
        } else {
            printf("    %d: %d\n", i, sps.dist[i]);
        }
    }

    // Printing the predecessor(s) of each vertex in the graph
    printf("    Preds\n");
    struct PredNode **current = sps.pred;
    for (int i = 0; i < sps.numNodes; i++) {
        printf("    %d: ", i);
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
    /*for (int i = 0; i < sps.numNodes; i++) {*/
        /*struct PredNode *current = sps.pred[i];*/
        /*sps.pred[i] = sps.pred[i]->next;*/
        /*free(current);*/
    /*}*/

    // Free dist array
    free(sps.dist);
}
