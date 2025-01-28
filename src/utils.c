#include "../include/utils.h"
#include <igraph/igraph.h>
#include <mpi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MAX
#define MAX 200
#endif

#ifndef MIN
#define MIN 1
#endif

extern double program_start_time;

void print_debug(const char *format, const char *color, const int rank, ...) {
  if (DEBUG) {
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


void generate_complete_graph(const int V, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "Error opening file.\n");
    exit(1);
  }

  // generating a complete graph with V vertices and without self loops
  igraph_t graph;
  igraph_integer_t V_igraph = V;
  igraph_full(&graph, V_igraph, IGRAPH_UNDIRECTED, IGRAPH_NO_LOOPS);

  // NOTE: is this better than get_random() or rand()?
  // the answer is left to the whoever wants to lose time
  FILE *random = fopen("/dev/urandom", "r");
  if (random == NULL) {
    fprintf(stderr, "Error opening /dev/urandom.\n");
    exit(1);
  }

  unsigned int random_value;

  // print graph to file
  fprintf(file, "%d %d\n", V, V * (V - 1) / 2);

  igraph_integer_t from, to;
  igraph_vector_int_t edges;
  igraph_integer_t E = igraph_ecount(&graph);
  igraph_vector_int_init(&edges, E);
  igraph_get_edgelist(&graph, &edges, 0);
  for (int i = 0; i < E; i++) {
    fread(&random_value, sizeof(unsigned int), 1, random);
    from = VECTOR(edges)[2 * i];
    to = VECTOR(edges)[2 * i + 1];
    fprintf(file, "%d %d %d\n", (int)from, (int)to, (random_value % MAX) + MIN);
  }
  igraph_vector_int_destroy(&edges);

  fclose(file);

  igraph_destroy(&graph);
}

void Bcast_adj_graph(AG **g, MPI_Comm comm) {
  int V, E;
  if (*g != NULL) {
    V = (*g)->V;
    E = (*g)->E;
  }
  MPI_Bcast(&V, 1, MPI_INT, 0, comm);
  MPI_Bcast(&E, 1, MPI_INT, 0, comm);

  if (*g == NULL) {
    *g = init_adj_graph(V, E);
  }

  // MPI_Request request;
  // MPI_Ibcast(( *g )->edges, E * 3, MPI_INT, 0, comm, &request);
  // MPI_Wait(&request, MPI_STATUS_IGNORE);
  MPI_Bcast((*g)->edges, E * 3, MPI_INT, 0, comm);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank != 0) {
    print_debug("Graph broadcasted. V = %d, E = %d", ANSI_COLOR_GREEN, rank, V,
                E);
    print_debug("V = %d, E = %d", ANSI_COLOR_CYAN, rank, V, E);
  }
}
