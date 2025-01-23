#ifndef MST_H
#define MST_H

#include "graph.h"
void find_min_edge(G*);
void remove_mirror_edges(G*);
void propagate_colors(G*);
void new_vertex_ids(G*);
void contract_graph(G*);

#endif // !MST_H
