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

#ifndef INPUT
#define INPUT "graph.txt"
#endif

#ifndef OUTPUT
#define OUTPUT "mst.txt"
#endif

double program_start_time;

int main(int argc, char *argv[]) {
  srand(time(NULL));

  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  program_start_time = MPI_Wtime();

  AG *g = NULL;
  AG *mst = NULL;

  int graph_generated = 0;

  if (rank == 0) {
    switch (GEN) {
      case 0:
        debug("Loading graph from %s ...", ANSI_COLOR_YELLOW, rank, INPUT);
        g = init_from_file(INPUT);
        debug("Graph loaded.", ANSI_COLOR_GREEN, rank);
        graph_generated = 1;
        break;

      default:
        g = generate_graph(GEN);
        if (SAVE) { 
          debug("Saving graph to %s ...", ANSI_COLOR_YELLOW, rank, INPUT);
          print_file_adj_graph(g, INPUT); 
          debug("Graph saved.", ANSI_COLOR_GREEN, rank);
        }
        graph_generated = 1;
        break;
    }
  } else {
    graph_generated = 1;
  }


  MPI_Bcast(&graph_generated, 1, MPI_INT, 0, MPI_COMM_WORLD);


  if (graph_generated && rank == 0) {
    Bcast_adj_graph(&g, MPI_COMM_WORLD);
  } else {
    Bcast_adj_graph(&g, MPI_COMM_WORLD);
  }

  debug("V = %d, E = %d", ANSI_COLOR_CYAN, rank, g->V, g->E);
  mst = init_adj_graph(g->V, g->V - 1);
  // not needed but just to be sure
  MPI_Barrier(MPI_COMM_WORLD);
  // debug("Process reached barrier", ANSI_COLOR_YELLOW, rank);

  double start_time;
  debug("Starting Boruvka...", ANSI_COLOR_MAGENTA, rank);
  start_time = MPI_Wtime();
  adj_boruvka(g, mst);

  if (rank == 0) {
    double end_time = MPI_Wtime();
    printf("[END] %s MST Time: %f %s\n", ANSI_COLOR_BLUE, end_time - start_time, ANSI_COLOR_RESET);

    if (SAVE) print_file_mst(mst, OUTPUT);
  }

  // clean up
  free_adj_graph(g);
  free_adj_graph(mst);

  MPI_Finalize();
  return 0;
}
