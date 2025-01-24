#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>

AG *init_adj_graph(const int V, const int E) {
  AG *g = (AG *)malloc(sizeof(AG));
  if (g == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for graph\n");
    exit(1);
  }

  g->V = V;
  g->E = E;

  g->edges = (Edge *)malloc(E * sizeof(Edge));
  if (g->edges == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for graph\n");
    exit(1);
  }

  return g;
}

void free_adj_graph(AG *g) {
  free(g->edges);
  free(g);
}

AG *init_from_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error: unable to open file\n");
    exit(1);
  }

  int V, E;
  fscanf(file, "%d %d", &V, &E);
  AG *g = init_adj_graph(V, E);

  for (int i = 0; i < E; i++) {
    fscanf(file, "%d %d %d", &g->edges[i].src, &g->edges[i].dest,
           &g->edges[i].w);
  }

  fclose(file);

  return g;
}

Edge clone_edge(Edge *e) {
  Edge clone;
  clone.src = e->src;
  clone.dest = e->dest;
  clone.w = e->w;
  return clone;
}

void print_file_adj_graph(AG *g, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "Error: unable to open file\n");
    exit(1);
  }

  fprintf(file, "%d %d\n", g->V, g->E);
  for (int i = 0; i < g->E; i++) {
    fprintf(file, "%d %d %d\n", g->edges[i].src, g->edges[i].dest,
            g->edges[i].w);
  }

  fclose(file);
}


CSRG *init_csr_graph(const int V, const int E) {
  CSRG *g = (CSRG *)malloc(sizeof(CSRG));
  if (g == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for graph\n");
    exit(1);
  }

  g->V = V;
  g->E = E;

  g->dest = (int *)malloc(E * sizeof(int));
  g->w = (int *)malloc(E * sizeof(int));
  g->fe = (int *)malloc(V * sizeof(int));
  g->od = (int *)malloc(V * sizeof(int));

  if (g->dest == NULL || g->w == NULL || g->fe == NULL || g->od == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for graph\n");
    exit(1);
  }

  return g;
}

void free_csr_graph(CSRG *g) {
  free(g->dest);
  free(g->w);
  free(g->fe);
  free(g->od);
  free(g);
}

CSRG *clone_graph(CSRG *g) {
  CSRG *clone = init_csr_graph(g->V, g->E);

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
