#include "../include/graph.h"
#include "../include/mst.h"
#include "../include/utils.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

double program_start_time;
int verbose = 0;
int max = 50000;
int min = 1;

int main(int argc, char *argv[]) {
  srand(time(NULL));

  int gen = 0;
  int save = 0;
  int save_output = 0;
  char *input = malloc(256 * sizeof(char));
  char *output = malloc(256 * sizeof(char));

  input = "graph.txt";
  output = "mst.txt";

  int rank, size;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-g") == 0) {
      gen = atoi(argv[i + 1]);
      i++;
    } else if (strcmp(argv[i], "-v") == 0) {
      verbose = 1;
    } else if (strcmp(argv[i], "-s") == 0) {
      save = 1;
    } else if (strcmp(argv[i], "-i") == 0) {
      input = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-o") == 0) {
      save_output = 1;
      output = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-max") == 0) {
      max = atoi(argv[i + 1]);
      i++;
    } else if (strcmp(argv[i], "-min") == 0) {
      min = atoi(argv[i + 1]);
      i++;
    }
  }

  program_start_time = MPI_Wtime();

  AG *g = NULL;
  AG *mst = NULL;

  int graph_generated = 0;

  if (rank == 0) {
    switch (gen) {
      case 0:
        debug("Loading graph from %s ...", ANSI_COLOR_YELLOW, rank, input);
        g = init_from_file(input);
        debug("Graph loaded.", ANSI_COLOR_GREEN, rank);
        graph_generated = 1;
        break;

      default:
        g = generate_graph(gen);
        if (save) { 
          debug("Saving graph to %s ...", ANSI_COLOR_YELLOW, rank, input);
          print_file_adj_graph(g, input); 
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
  adj_boruvka(g, mst, rank, size, MPI_COMM_WORLD);

  if (rank == 0) {
    double end_time = MPI_Wtime();
    // printf("[END] %s MST Time: %f %s\n", ANSI_COLOR_BLUE, end_time - start_time, ANSI_COLOR_RESET);
    printf("%f\n", end_time - start_time);

    if (save || save_output) print_file_mst(mst, output);
  }

  // clean up
  // free_adj_graph(g);
  free(g);
  free_adj_graph(mst);

  MPI_Finalize();
  return 0;
}
