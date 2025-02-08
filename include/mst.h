#ifndef MST_H
#define MST_H

#include "graph.h"
#include <mpi.h>

/*
* Adjacency list implementation
* @param `g`: pointer to the original graph
* @param `mst`: pointer to the minimum spanning tree
* @param `rank`: rank of the process
* @param `size`: number of processes
* @param `comm`: MPI communicator
* @return `AG*`: pointer to the minimum spanning tree
*/
void adj_boruvka(AG*, AG*, int, int, MPI_Comm);

// void find_min_edge(CSRG*);
// void remove_mirror_edges(CSRG*);
// void propagate_colors(CSRG*);
// void new_vertex_ids(CSRG*);
// void contract_graph(CSRG*);

#endif // !MST_H
