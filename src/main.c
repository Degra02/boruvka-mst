#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"
#include "../include/mst.h"
#include "../include/utils.h"

#ifndef GEN
  #define GEN 0
#endif

// #ifndef V_GEN
//   #define V_GEN 1000
// #endif

int main(int argc, char *argv[]) {
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

  AG *g = init_from_file(argv[1]);
  AG *mst = init_adj_graph(g->V, g->V - 1);

  double start_time, end_time;

  start_time = MPI_Wtime();
  adj_boruvka(g, mst);
  end_time = MPI_Wtime();
  
  if (rank == 0) {
    printf("Time: %f\n", end_time - start_time);

    print_file_adj_graph(mst, argv[2]);

    free_adj_graph(g);
    free_adj_graph(mst);
  }

  MPI_Finalize();
  return 0;
}
