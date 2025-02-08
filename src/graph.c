#include "../include/graph.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void create_edge_mpi_type(MPI_Datatype *MPI_Edge) {
  int blocklengths[3] = {1, 1, 1};
  MPI_Datatype types[3] = {MPI_INT, MPI_INT, MPI_INT};
  MPI_Aint offsets[3];

  offsets[0] = offsetof(Edge, src);
  offsets[1] = offsetof(Edge, dest);
  offsets[2] = offsetof(Edge, w);

  MPI_Type_create_struct(3, blocklengths, offsets, types, MPI_Edge);
  MPI_Type_commit(MPI_Edge);
}

AG *init_adj_graph(const uint32_t V, const uint32_t E) {
  AG *g = (AG *)malloc(sizeof(AG));
  if (g == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for graph\n");
    exit(1);
  }

  g->V = V;
  g->E = E;

  g->edges = (uint32_t *)calloc(E * 3, sizeof(uint32_t));
  // MPI_Alloc_mem(E * 3 * sizeof(int), MPI_INFO_NULL, &g->edges);
  if (g->edges == NULL) {
    fprintf(stderr, "Error: unable to allocate memory for graph's edges\n");
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

  uint32_t V, E;
  fscanf(file, "%d %d", &V, &E);
  AG *g = init_adj_graph(V, E);

  for (uint32_t i = 0; i < E; i++) {
    fscanf(file, "%d %d %d", &g->edges[i*3], &g->edges[i*3 + 1],
           &g->edges[i*3 + 2]);
  }

  fclose(file);

  return g;
}


void print_file_adj_graph(AG *g, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "Error: unable to open file\n");
    exit(1);
  }

  fprintf(file, "%d %d\n", g->V, g->E);
  for (uint32_t i = 0; i < g->E; i++) {
    fprintf(file, "%d %d %d\n", g->edges[i*3], g->edges[i*3 + 1],
            g->edges[i*3 + 2]);
  }

  fclose(file);
}

void print_file_mst(AG *mst, const char* filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "Error: unable to open file\n");
    exit(1);
  }

  int sum = 0;
  fprintf(file, "%d %d\n", mst->V, mst->E);
  for (uint32_t i = 0; i < mst->E; i++) {
    sum += mst->edges[i*3 + 2];
    fprintf(file, "%d %d %d\n", mst->edges[i*3], mst->edges[i*3 + 1],
            mst->edges[i*3 + 2]);
  }
  // fprintf(file, "Total weight: %d\n", sum);

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
