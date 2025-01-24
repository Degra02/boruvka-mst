#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

#include <igraph/igraph.h>

void generate_complete_graph(const int V, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "Error opening file.\n");
    exit(1);
  }

  // generating a complete graph with V vertices and without self loops
  igraph_t graph;
  igraph_integer_t V_igraph = V;
  igraph_full(&graph, V_igraph, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);


  // print graph to file
  fprintf(file, "%d %d\n", V, V*V - V);

  igraph_integer_t from, to;
  igraph_vector_int_t edges;
  igraph_vector_int_init(&edges, 2 * V * V);
  igraph_get_edgelist(&graph, &edges, 0);
  for (int i = 0; i < V; i++) {
    from = VECTOR(edges)[2 * i];
    to = VECTOR(edges)[2 * i + 1];
    fprintf(file, "%d %d %d\n", (int)from, (int)to, 1);
  }

  fclose(file);

  igraph_vector_int_destroy(&edges);
  igraph_destroy(&graph);
}
