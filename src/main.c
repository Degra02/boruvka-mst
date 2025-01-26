#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"
#include "../include/mst.h"
#include "../include/utils.h"
#include <time.h>

#ifndef GEN
  #define GEN 0
#endif

#ifndef SAVE
  #define SAVE 0
#endif

int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <graph-file> <output-file>\n", argv[0]);
    exit(1);
  }

  if (GEN > 0) {
    printf("Generating complete graph with %d vertices.\n", GEN);
    generate_complete_graph(GEN, argv[1]);
  } 

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  AG *g = NULL;
  AG *mst = NULL;

  // FIX: Load graph only in rank 0
  // Currently the program crashes if the graph is not loaded in all ranks
  // if (rank == 0) {
    g = init_from_file(argv[1]);
    mst = init_adj_graph(g->V, g->V - 1);
    printf("Graph loaded.\n");
    // Bcast_adj_graph(g, MPI_COMM_WORLD);
  // } else {
  //   Bcast_adj_graph(g, MPI_COMM_WORLD);
  // }

  if (rank == 1) {
    printf("Rank 1\n");
    printf("V: %d\n", g->V);
  }

  double start_time;

  start_time = MPI_Wtime();
  adj_boruvka(g, mst);
  
  if (rank == 0) {
    double end_time = MPI_Wtime();
    printf("Time: %f\n", end_time - start_time);

    if (SAVE) print_file_adj_graph(mst, argv[2]);

    free_adj_graph(g);
    free_adj_graph(mst);
  }

  MPI_Finalize();
  return 0;
}
