//
// Created by Rui.Mu on 2019-01-09.
//

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "WGraph.h"

typedef struct GraphRep {
    int **edges;    // adjacency matrix
    int nV;         // vertices;
    int nE;         // edges
} GraphRep;

Graph newGraph(int V) {
    assert(V >= 0);
    int i;

    Graph g = malloc(sizeof(GraphRep));

    assert(g != NULL);
    // vertices
    g->nV = V;
    // edges
    g->nE = 0;

    // allocate memory for each row
    g->edges = malloc(V * sizeof(int *));
    assert(g->edges != NULL);

    for (i = 0; i < V; i++) {
        g->edges[i] = calloc(V, sizeof(int));
        assert(g->edges[i] != NULL);
        for (int j = 0; j < V; j++) {
            g->edges[i][j] = -1;
        }
    }

    return g;
}

/**
 * check value vertices
 * @param g
 * @param v
 */
bool validV(Graph g, Vertex v) {
    return (g != NULL && v >= 0 && v < g->nV);
}

/**
 * insert edge
 * @param g graph
 * @param e edge
 */
void insertEdge(Graph g, Edge e) {
    if (g->edges[e.v][e.w] == -1) {
        g->edges[e.v][e.w] = e.duration;
        g->nE++;
    }

}

/**
 * remove the edge
 * @param g
 * @param e
 */
void removeEdge(Graph g, Edge e) {
    if (g->edges[e.v][e.w] > -1) {
        g->edges[e.v][e.w] = -1;
        g->nE--;
    }
}

/**
 * check adjacent
 * @param g
 * @param x
 * @param w
 * @return
 */
bool adjacent(Graph g, Vertex v, Vertex w) {

    assert(g != NULL && validV(g, v) && validV(g, w));

    return (g->edges[v][w] != -1);
}


/**
 * get graph cost
 * @return
 */
int getGraphCost(Graph g, Vertex v, Vertex w) {
    assert(g != NULL && validV(g, v) && validV(g, w));
    return g->edges[v][w];
}

void showGraph(Graph g) {
    assert(g != NULL);
    int i, j;

    for (i = 0; i < g->nV; i++) {
        int count = 0;
        for (j = 0; j < g->nV; j++) {
            if (g->edges[i][j]) {
                if (count == 0) {
                    printf("<%d %d>", i, j);
                } else {
                    printf(" <%d %d>", i, j);
                }

                count++;
            }
        }
        if (count > 0) {
            printf("\n");
        }
    }
}

/**
 * degree number of vertex
 * @return number degree of vertex
 */
int degreeOfIn(Graph g, Vertex v) {
    assert(g != NULL);
    int i;
    int result = 0;
    for (i = 0; i < g->nV; i++) {
        if (adjacent(g, i, v) == 1) {
            result++;
        }
    }
    return result;
}

/**
 * degree number of vertex
 * @return number degree of vertex
 */
int degreeOfOut(Graph g, Vertex v) {
    assert(g != NULL);
    int i;
    int result = 0;
    for (i = 0; i < g->nV; i++) {
        if (adjacent(g, v, i) == 1) {
            result++;
        }
    }
    return result;
}

void freeGraph(Graph g) {
    assert(g != NULL);
    int i;
    for (i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

