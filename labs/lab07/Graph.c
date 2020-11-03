// Implementation of the undirected weighted graph ADT

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

#define VISITED 1

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;      // #vertices
	int nE;      // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

static int validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV)
{
	assert(nV > 0);

	Graph new = malloc(sizeof(*new));
	assert(new != NULL);
	new->nV = nV;
	new->nE = 0;

	new->edges = calloc(nV, sizeof(int *));
	assert(new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc(nV, sizeof(int));
		assert(new->edges[v] != 0);
	}

	return new;
}

void GraphFree(Graph g)
{
	assert(g != NULL);
	for (int v = 0; v < g->nV; v++)
		free(g->edges[v]);
	free(g->edges);
	free(g);
}

////////////////////////////////////////////////////////////////////////

void GraphInsertEdge(Graph g, Vertex v, Vertex w, int weight)
{
	assert(g != NULL && validVertex(g, v) && validVertex(g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		return; // an edge already exists; do nothing.

	g->edges[v][w] = weight;
	g->edges[w][v] = weight;
	g->nE++;
}

void GraphRemoveEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validVertex(g, v) && validVertex(g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		return; // the edge doesn't exist; do nothing.

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

////////////////////////////////////////////////////////////////////////

/**
 * Builds the shortest path and updates path array given a
 * predecessor array and the destination vertice
 */
int buildPath(Graph g, int *path, int *pred, Vertex dest) {
    int *reversedArray = calloc(g->nV, sizeof(int));

    // Traceback path and update shortest path array
    int numV = 0;
    for (int v = dest; v != -1; v = pred[v]) {
        reversedArray[numV] = v;
        numV++;
    }

    // Reverse the reversed array
    for (int i = 0, j = (numV - 1); i < numV; i++, j--) path[i] = reversedArray[j];

    return numV;
}


/**
 * Finds  the  shortest path (in terms of the number of hops) from `src`
 * to `dest` such that no edge on the path has weight larger than `max`.
 * Stores  the  path  in the given `path` array including both `src` and
 * `dest` and returns the number of vertices stored in the  path  array.
 * Returns 0 if there is no such path.
 */
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
    // Create predecessor list and initialise values as -1
    int *pred = malloc(g->nV * sizeof(int));
    for (int i = 0; i < g->nV; i++) pred[i] = -1;

    // Create a visited adjacency matrix for people in network
    int *visited = calloc(g->nV, sizeof(int));

    // Create queue for BFS
    Queue q = QueueNew();
    QueueEnqueue(q, src);
    visited[src] = VISITED;

    while (!QueueIsEmpty(q)) {
        int v = QueueDequeue(q);

        // If path found, end while loop
        if (v == dest) break;

        // Find all edges to current vertice
        for (int w = 0; w < g->nV; w++) {
            if (g->edges[v][w] && !visited[w] && g->edges[v][w] <= max) {
                QueueEnqueue(q, w);
                // Update visited and 'predecessor' array
                visited[w] = VISITED;
                pred[w] = v;
            }
        }
    }

    QueueFree(q);
    free(visited);

    // Update shortest path
    int numVertices = buildPath(g, path, pred, dest);
	
    // Only return the number of vertices 
    if (path[0] == src) return numVertices;

    return 0;
}

////////////////////////////////////////////////////////////////////////

void GraphShow(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

static int validVertex(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

