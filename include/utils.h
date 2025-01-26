#include "graph.h"
#include <mpi.h>

#ifndef UTILS_H
#define UTILS_H

#ifndef DEBUG
  #define DEBUG 0
#endif

/*
* Generate a complete graph with V vertices.
* @param V Number of vertices.
* @param filename Name of the file to save the graph.
*/
void generate_complete_graph(const int, const char *);

/*
* Broadcasts an adjacency graph.
* @param g Pointer to the adjacency graph.
*/
void Bcast_adj_graph(AG **, MPI_Comm);

#endif // !UTILS_H
#define UTILS_H
