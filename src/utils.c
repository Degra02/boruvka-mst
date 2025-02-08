#include "../include/utils.h"
#include <mpi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

extern double program_start_time;
extern int verbose;
extern int max;
extern int min;

void debug(const char *format, const char *color, const int rank, ...) {
  if (verbose) {
    va_list args;
    va_start(args, rank);

    double current_time = MPI_Wtime();
    double time_elapsed = current_time - program_start_time;

    printf("%s[%f]%s ", ANSI_COLOR_GREY, time_elapsed, ANSI_COLOR_RESET);
    printf("%d: %s", rank, color);
    vprintf(format, args);
    printf("%s\n", ANSI_COLOR_RESET);

    va_end(args);
  }
}

unsigned int get_random() {
    unsigned int random_value;
    __asm__ __volatile__ (
        "rdrand %0"
        : "=r"(random_value)
        : 
        : "cc"
    );
    return random_value;
}

AG* generate_graph(const int V) {
  AG *g = init_adj_graph(V, V * (V - 1) / 2);
  debug("Generating graph. V = %d, E = %d", ANSI_COLOR_GREEN, 0, V, V * (V - 1) / 2);

  FILE *random = fopen("/dev/urandom", "r");
  if (random == NULL) {
    fprintf(stderr, "Error opening /dev/urandom.\n");
    exit(1);
  }

  debug("Assigning random weights...", ANSI_COLOR_YELLOW, 0);
  unsigned int random_value;
  for (int i = 0, k = 0; i < V; i++) {
    for (int j = i + 1; j < V; j++, k++) {
      fread(&random_value, sizeof(unsigned int), 1, random);
      g->edges[k*3] = i;
      g->edges[k*3 + 1] = j;
      g->edges[k*3 + 2] = (random_value % max) + min;
    }
  }
  debug("Random weights assigned.", ANSI_COLOR_GREEN, 0);

  return g;
}

void Bcast_adj_graph(AG **g, MPI_Comm comm) {
  int V, E;
  if ((*g) != NULL) {
    debug("Broadcasting graph V = %d, E = %d", ANSI_COLOR_YELLOW, 0, (*g)->V, (*g)->E);
    V = (*g)->V;
    E = (*g)->E;
  }
  MPI_Bcast(&V, 1, MPI_INT, 0, comm);
  MPI_Bcast(&E, 1, MPI_INT, 0, comm);

  if ((*g) == NULL) {
    *g = init_adj_graph(V, E);
  }

  // MPI_Request request;
  // MPI_Ibcast(( *g )->edges, E * 3, MPI_INT, 0, comm, &request);
  // MPI_Wait(&request, MPI_STATUS_IGNORE);
  // MPI_Bcast((*g)->edges, E * 3, MPI_INT, 0, comm);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank != 0) {
    debug("Graph broadcasted. V = %d, E = %d", ANSI_COLOR_GREEN, rank, V,
                E);
    debug("V = %d, E = %d", ANSI_COLOR_CYAN, rank, V, E);
  }
}
