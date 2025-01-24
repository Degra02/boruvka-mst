#ifndef MST_H
#define MST_H

#include "graph.h"

/*
* Adjacency list implementation
* @param `g`: pointer to the original graph
* @param `mst`: pointer to the minimum spanning tree
* @return `AG*`: pointer to the minimum spanning tree
*/
void adj_boruvka(AG*, AG*);

// void find_min_edge(CSRG*);
// void remove_mirror_edges(CSRG*);
// void propagate_colors(CSRG*);
// void new_vertex_ids(CSRG*);
// void contract_graph(CSRG*);

#endif // !MST_H
