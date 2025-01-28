#include "../include/graph.h"
#include "../include/mst.h"
#include "../include/utils.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef GEN
#define GEN 0
#endif

#ifndef SAVE
#define SAVE 0
#endif

#ifndef OMP
#define OMP 0
#endif

double program_start_time;

int main(int argc, char *argv[]) {
  srand(time(NULL));
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <graph-file> <output-file>\n", argv[0]);
    exit(1);
  }

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  program_start_time = MPI_Wtime();
  AG *g = NULL;
  AG *mst = NULL;


  // TODO: add choice between generating graph.txt file and loading it 
  // or just generating the graph without saving it to a file
  int graph_generated = 0;
  if (GEN > 0 && rank == 0) {
    // generate_complete_graph(GEN, argv[1]);
    g = generate_graph(GEN);
    graph_generated = 1;
  } else if (GEN == 0) {
    graph_generated = 1;
  }

  MPI_Bcast(&graph_generated, 1, MPI_INT, 0, MPI_COMM_WORLD);


  if (graph_generated && rank == 0) {
    // g = init_from_file(argv[1]);
    // print_debug("Graph loaded.", ANSI_COLOR_GREEN, rank);
    Bcast_adj_graph(&g, MPI_COMM_WORLD);
  } else {
    Bcast_adj_graph(&g, MPI_COMM_WORLD);
  }

  print_debug("V = %d, E = %d", ANSI_COLOR_CYAN, rank, g->V, g->E);
  mst = init_adj_graph(g->V, g->V - 1);
  // not needed but just to be sure
  MPI_Barrier(MPI_COMM_WORLD);
  // print_debug("Process reached barrier", ANSI_COLOR_YELLOW, rank);

  double start_time;
  print_debug("Starting Boruvka...", ANSI_COLOR_MAGENTA, rank);
  start_time = MPI_Wtime();
  adj_boruvka(g, mst);

  if (rank == 0) {
    double end_time = MPI_Wtime();
    printf("[END] %s MST Time: %f %s\n", ANSI_COLOR_BLUE, end_time - start_time, ANSI_COLOR_RESET);

    if (SAVE) print_file_adj_graph(mst, argv[2]);
  }
  free_adj_graph(g);
  free_adj_graph(mst);

  MPI_Finalize();
  return 0;
}
