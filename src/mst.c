#include <limits.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "../include/mfset.h"
#include "../include/mst.h"

void adj_boruvka(AG *g, AG *mst) {
  int rank;
  int size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int V, E;

  // broadcast V and E
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

  if (rank == size - 1 && E % edges_per_proc != 0) {
    edges_per_proc = E % edges_per_proc;
  }

  // TODO: check if the size is compatible with the number of processes

  // HACK: should MPI_Alloc_mem be used?
  MFSet *mfset = init_mfset(V);

  int mst_edges = 0;
  Edge *closest = (Edge *)malloc(V * sizeof(Edge));
  Edge *closest_local = (Edge *)malloc(V * sizeof(Edge));

  for (int i = 1; i < V && mst_edges < V - 1; i *= 2) {
    #pragma omp parallel for
    for (int j = 0; j < V; j++) {
      closest[j].w = INT_MAX;
    }

    // search for the closest edge
    // #pragma omp parallel for
    for (int j = 0; j < edges_per_proc; j++) {
      Edge e = local_edges[j];

      int root_src = find(mfset, e.src);
      int root_dest = find(mfset, e.dest);

      if (root_src == root_dest) {
        continue;
      }
      
      // #pragma omp critical
      {
        if (closest[root_src].w == INT_MAX || e.w < closest[root_src].w) {
          closest[root_src] = clone_edge(&e);
        }

        if (closest[root_dest].w == INT_MAX || e.w < closest[root_dest].w) {
          closest[root_dest] = clone_edge(&e);
        }
      }
    }

    int from, to;
    for (int st = 1; st < size; st *= 2) {
      if (rank % (2 * st) == 0) {
        from = rank + st;
        if (from < size) {
          MPI_Recv(closest_local, V * 3, MPI_INT, from, 0, MPI_COMM_WORLD,
                   MPI_STATUS_IGNORE);

          for (int j = 0; j < V; j++) {
            if (closest_local[j].w < closest[j].w) {
              closest[j] = clone_edge(&closest_local[j]);
            }
          }
        }
      } else if (rank % st == 0) {
        to = rank - st;
        MPI_Send(closest, V * 3, MPI_INT, to, 0, MPI_COMM_WORLD);
      }
    }

    MPI_Bcast(closest, V * 3, MPI_INT, 0, MPI_COMM_WORLD);

    // #pragma omp parallel for
    for (int j = 0; j < V; j++) {
      if (closest[j].w != INT_MAX) {
        int root_src = find(mfset, closest[j].src);
        int root_dest = find(mfset, closest[j].dest);

        if (root_src != root_dest) {
          // #pragma omp critical
          {
            // if (rank == 0) {
              mst->edges[mst_edges++] = clone_edge(&closest[j]);
            // }
            unite(mfset, root_src, root_dest);
          }
        }
      }
    }
  }

  free_mfset(mfset);
  free(closest);
  free(closest_local);
  free(local_edges);
}
