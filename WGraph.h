//
// Created by YUJiehu on 2019-02-02.
//

#ifndef COMP9024_GRAPH_H
#define COMP9024_GRAPH_H

#include <stdbool.h>

typedef struct GraphRep *Graph;

// vertices are ints
typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
    Vertex v;
    Vertex w;
    int startHour;
    int startMin;
    int endHour;
    int endMin;
    int duration;

} Edge;

Graph newGraph(int);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
bool  adjacent(Graph, Vertex, Vertex);
void  showGraph(Graph);
void  freeGraph(Graph);

/**
 * get graph cost
 * @return
 */
int getGraphCost(Graph,Vertex, Vertex);

/**
 * degree number of vertex
 * @return number degree of vertex
 */
int degreeOfIn(Graph, Vertex);

int degreeOfOut(Graph, Vertex);

#endif //COMP9024_GRAPH_H
