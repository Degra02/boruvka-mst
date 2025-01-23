#include <mpi.h>
#include <stdlib.h>

#include "../include/mst.h"
#include "../include/mfset.h"

AG *adj_boruvka(AG *g) {
  int rank;
  int size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int V, E;
  if (rank == 0) {
    V = g->V;
    E = g->E;
    MPI_Bcast(&V, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&E, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Bcast(&V, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&E, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }

  // split edges among processes
  int edges_per_proc = (E + size - 1) / size;
  Edge *local_edges = (Edge *)malloc(edges_per_proc * sizeof(Edge));
  MPI_Scatter(g->edges, edges_per_proc * 3, MPI_INT, local_edges,
              edges_per_proc * 3, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == size - 1 && E % size != 0) {
    edges_per_proc = E % size;
  }

  //TODO: check if the size is compatible with the number of processes

  MFSet *mfset = init_mfset(V);
  
  int mst_edges = 0;
  Edge *closest = (Edge *)malloc(V * sizeof(Edge));




  MPI_Finalize();
}
