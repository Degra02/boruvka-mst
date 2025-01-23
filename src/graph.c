#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"

G* init_graph(const int V, const int E) {
  G* g = (G*)malloc(sizeof(G));
  if (g == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for graph\n");
    exit(1);
  }

  g->V = V;
  g->E = E;

  g->dest = (int*)malloc(E * sizeof(int));
  g->w = (int*)malloc(E * sizeof(int));
  g->fe = (int*)malloc(V * sizeof(int));
  g->od = (int*)malloc(V * sizeof(int));

  if (g->dest == NULL || g->w == NULL || g->fe == NULL || g->od == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for graph\n");
    exit(1);
  }

  return g;
}

void free_graph(G* g) {
  free(g->dest);
  free(g->w);
  free(g->fe);
  free(g->od);
  free(g);
}

G* clone_graph(G* g) {
  G* clone = init_graph(g->V, g->E);

  for (int i = 0; i < g->E; i++) {
    clone->dest[i] = g->dest[i];
    clone->w[i] = g->w[i];
  }

  for (int i = 0; i < g->V; i++) {
    clone->fe[i] = g->fe[i];
    clone->od[i] = g->od[i];
  }

  return clone;
}
