#include "../include/utils.h"
#include <mpi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

extern double program_start_time;
extern int verbose;
extern uint32_t max;
extern uint32_t min;

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

uint32_t get_random() {
    uint32_t random_value;
    __asm__ __volatile__ (
        "rdrand %0"
        : "=r"(random_value)
        :
        : "cc"
    );
    return random_value;
}

AG* generate_complete_graph(const uint32_t V) {
  AG *g = init_adj_graph(V, V * (V - 1) / 2);
  debug("Generating graph. V = %d, E = %d", ANSI_COLOR_GREEN, 0, V, V * (V - 1) / 2);

  FILE *random = fopen("/dev/urandom", "r");
  if (random == NULL) {
    fprintf(stderr, "Error opening /dev/urandom.\n");
    exit(1);
  }

  debug("Assigning random weights...", ANSI_COLOR_YELLOW, 0);
  uint32_t random_value;
  for (uint32_t i = 0, k = 0; i < V; i++) {
    for (uint32_t j = i + 1; j < V; j++, k++) {
      random_value = rand() % (max - min + 1) + min;
      g->edges[k*3] = i;
      g->edges[k*3 + 1] = j;
      g->edges[k*3 + 2] = random_value;
    }
  }
  debug("Random weights assigned.", ANSI_COLOR_GREEN, 0);

  return g;
}

AG* generate_graph(const uint32_t V, const double density) {
  // maximum number of edges
  AG *temp = init_adj_graph(V, V * (V - 1) / 2);

  FILE *random = fopen("/dev/urandom", "r");
  if (random == NULL) {
    fprintf(stderr, "Error opening /dev/urandom.\n");
    exit(1);
  }

  debug("Assigning random weights...", ANSI_COLOR_YELLOW, 0);
  uint32_t random_value;
  for (uint32_t i = 0, k = 0; i < V; i++) {
    for (uint32_t j = i + 1; j < V; j++, k++) {
      fread(&random_value, sizeof(unsigned int), 1, random);
      temp->edges[k*3] = i;
      temp->edges[k*3 + 1] = j;
      temp->edges[k*3 + 2] = (random_value % max) + min;
    }
  }

  // number of edges of new graph
  uint32_t E = (uint32_t)(V * (V - 1) / 2 * density);

  AG *g = init_adj_graph(V, E);
  debug("Generating graph. V = %d, E = %d", ANSI_COLOR_GREEN, 0, V, E);

  // remove some edges from temp graph
  for (uint32_t i = 0; i < E; i++) {
    uint32_t random_edge = get_random() % (V * (V - 1) / 2);
    g->edges[i*3] = temp->edges[random_edge*3];
    g->edges[i*3 + 1] = temp->edges[random_edge*3 + 1];
    g->edges[i*3 + 2] = temp->edges[random_edge*3 + 2];
  }

  free_adj_graph(temp);
  return g;
}


void Bcast_adj_graph(AG **g, MPI_Comm comm) {
  uint32_t V, E;
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
