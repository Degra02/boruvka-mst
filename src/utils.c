#include "../include/utils.h"
#include <igraph/igraph_interface.h>
#include <stdio.h>
#include <stdlib.h>
#include <igraph/igraph.h>

#define MAX_WEIGHT 200

void generate_complete_graph(const int V, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "Error opening file.\n");
    exit(1);
  }
  srand(time(NULL));

  // generating a complete graph with V vertices and without self loops
  igraph_t graph;
  igraph_integer_t V_igraph = V;
  igraph_full(&graph, V_igraph, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);


  // print graph to file
  fprintf(file, "%d %d\n", V, V * (V - 1) / 2);

  igraph_integer_t from, to;
  igraph_vector_int_t edges;
  igraph_integer_t E = igraph_ecount(&graph);
  igraph_vector_int_init(&edges, E);
  igraph_get_edgelist(&graph, &edges, 0);
  for (int i = 0; i < E; i++) {
    from = VECTOR(edges)[2 * i];
    to = VECTOR(edges)[2 * i + 1];
    fprintf(file, "%d %d %d\n", (int)from, (int)to, rand() % MAX_WEIGHT + 1);
  }
  igraph_vector_int_destroy(&edges);

  fclose(file);

  igraph_destroy(&graph);
}
