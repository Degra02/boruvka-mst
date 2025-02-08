#include <stdint.h>
#include <limits.h>
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/mfset.h"
#include "../include/mst.h"
#include "../include/utils.h"

#ifdef _OPENMP
#include <omp.h>
#endif

void adj_boruvka(AG *g, AG *mst, int rank, int size, MPI_Comm comm) {
  // int rank;
  // int size;
  //
  // MPI_Comm_rank(comm, &rank);
  // MPI_Comm_size(comm, &size);

  // MPI_Datatype edge_type;
  // create_edge_mpi_type(&edge_type);

  int V, E;
  V = g->V;
  E = g->E;

  // if (rank == 0) {
  //   MPI_Bcast(&V, 1, MPI_INT, 0, comm);
  //   MPI_Bcast(&E, 1, MPI_INT, 0, comm);
  // } else {
  //   MPI_Bcast(&V, 1, MPI_INT, 0, comm);
  //   MPI_Bcast(&E, 1, MPI_INT, 0, comm);
  // }

  // split edges among processes
  uint32_t edges_per_proc = (E + size - 1) / size;
  int *local_edges;
  local_edges = (int *)malloc(edges_per_proc * 3 * sizeof(int));
  // MPI_Alloc_mem(edges_per_proc * sizeof(Edge), MPI_INFO_NULL, &local_edges);

  if (local_edges == NULL) {
    debug("Failed to allocate memory for local edges", ANSI_COLOR_RED, rank);
    MPI_Abort(comm, 1);
  } else {
    debug("Allocated memory for local edges", ANSI_COLOR_CYAN, rank);
  }

  if (rank == 0)
    debug("Starting scatter of edges", ANSI_COLOR_CYAN, rank);

  int scatter_res =
    MPI_Scatter(g->edges, edges_per_proc * 3, MPI_INT, local_edges,
                edges_per_proc * 3, MPI_INT, 0, comm);
  if (scatter_res != MPI_SUCCESS) {
    debug("Failed to scatter edges", ANSI_COLOR_RED, rank);
    MPI_Abort(comm, 1);
  }

  // safe to free the original edges
  free(g->edges);

  // if (rank == 0) {
  //   int scatter_res =
  //       MPI_Scatter(g->edges, edges_per_proc, edge_type , local_edges,
  //                   edges_per_proc, edge_type, 0, comm);
  //   if (scatter_res != MPI_SUCCESS) {
  //     debug("Failed to scatter edges", ANSI_COLOR_RED, rank);
  //     MPI_Abort(comm, 1);
  //   }
  //
  //   free(g->edges);
  // } else {
  //   int scatter_res =
  //       MPI_Scatter(NULL, edges_per_proc, edge_type, local_edges,
  //                   edges_per_proc, edge_type, 0, comm);
  //   if (scatter_res != MPI_SUCCESS) {
  //     debug("Failed to scatter edges", ANSI_COLOR_RED, rank);
  //     MPI_Abort(comm, 1);
  //   }
  // }

  if (rank != 0)
    debug("Finished receiving edges", ANSI_COLOR_GREEN, rank);

  if (rank == size - 1 && E % edges_per_proc != 0) {
    edges_per_proc = E % edges_per_proc;
  }
  // debug("edges_per_proc = %d", ANSI_COLOR_CYAN, rank, edges_per_proc);

  MFSet *mfset = init_mfset(V);

  int mst_edges = 0;
  int *closest;
  int *closest_local;
  closest = (int *)malloc(V * 3 * sizeof(int));
  closest_local = (int *)malloc(V * 3 * sizeof(int));

  debug("Allocating memory for closest edges", ANSI_COLOR_CYAN, rank);
  // MPI_Alloc_mem(V * 3 * sizeof(int), MPI_INFO_NULL, &closest);
  // MPI_Alloc_mem(V * 3 * sizeof(int), MPI_INFO_NULL, &closest_local);

  MPI_Barrier(comm);

  for (int i = 1; i < V && mst_edges < V - 1; i *= 2) {
    #pragma omp parallel for
    for (int j = 0; j < V; j++) {
      closest[j*3 + 2] = INT_MAX;  
    }

    // search for the closest edge
    #pragma omp parallel for
    for (int j = 0; j < edges_per_proc; j++) {
      int *e = &local_edges[j * 3];

      int root_src = find(mfset, e[0]);
      int root_dest = find(mfset, e[1]);

      if (root_src == root_dest) {
        continue;
      }

      if (e[2] < closest[root_src*3 + 2]) {
        #pragma omp critical
        clone_edge(e, &closest[root_src * 3]);
      }

      if (e[2] < closest[root_dest*3 + 2]) {
        #pragma omp critical
        clone_edge(e, &closest[root_dest * 3]);
      }
    }

    int from, to;
    // MPI_Request request;
    for (int st = 1; st < size; st *= 2) {
      if (rank % (2 * st) == 0) {
        from = rank + st;
        if (from < size) {
          MPI_Recv(closest_local, V * 3, MPI_INT, from, 0, comm,
                   MPI_STATUS_IGNORE);
          // MPI_Irecv(closest_local, V * 3, MPI_INT, from, 0, comm,
          //           &request);
          // MPI_Wait(&request, MPI_STATUS_IGNORE);

          // static scheduling seems to be the best option, as the workload
          // is the same for each iteration
          #pragma omp parallel for schedule(static)
          for (int j = 0; j < V; j++) {
            if (closest_local[j*3 + 2] < closest[j*3 + 2]) {
              #pragma omp critical
              clone_edge(&closest_local[j*3], &closest[j*3]);
            }
          }
        }
      } else if (rank % st == 0) {
        to = rank - st;
        MPI_Send(closest, V * 3, MPI_INT, to, 0, comm);
        // MPI_Isend(closest, V * 3, MPI_INT, to, 0, comm, &request);
      }
    }

    debug("Broadcasting closest edges", ANSI_COLOR_CYAN, rank);
    MPI_Bcast(closest, V * 3, MPI_INT, 0, comm);

    for (int j = 0; j < V; j++) {
      if (closest[j*3 + 2] != INT_MAX) {
        {
          int root_src = find(mfset, closest[j*3]);
          int root_dest = find(mfset, closest[j*3 + 1]);

          if (root_src != root_dest) {
            clone_edge(&closest[j], &mst->edges[mst_edges]);
            mst_edges++;
            unite(mfset, root_src, root_dest);
          }
        }
      }
    }
  }

  // MPI_Free_mem(local_edges);
  // MPI_Free_mem(closest);
  // MPI_Free_mem(closest_local);

  free_mfset(mfset);
  free(closest);
  free(closest_local);
  free(local_edges);
}

// #pragma omp parallel
// {
//   Edge *local_closest = (Edge *)malloc(V * sizeof(Edge));
//   for(int j = 0; j < V; j++) {
//     local_closest[j].w = INT_MAX;
//   }
//
//   #pragma omp for
//   for(int j = 0; j < edges_per_proc; j++) {
//     Edge e = local_edges[j];
//
//     int root_src = find(mfset, e.src);
//     int root_dest = find(mfset, e.dest);
//
//     if (root_src != root_dest) {
//       if (e.w < local_closest[root_src].w) {
//         clone_edge(&e, &local_closest[root_src]);
//       }
//
//       if (e.w < local_closest[root_dest].w) {
//         clone_edge(&e, &local_closest[root_dest]);
//       }
//     }
//   }
//
//   #pragma omp critical
//   {
//     for(int j = 0; j < V; j++) {
//       if(local_closest[j].w < closest[j].w) {
//         clone_edge(&local_closest[j], &closest[j]);
//       }
//     }
//   }
//   free(local_closest);
// }

// #ifdef _OPENMP

// int *mst_edge_counts = (int *)calloc(omp_get_max_threads(), sizeof(int));
//
// #pragma omp parallel
// {
//   int thread_id = omp_get_thread_num();
//   int local_mst_edges = 0;
//
//   #pragma omp for
//   for (int j = 0; j < V; j++) {
//     if (closest[j].w != INT_MAX) {
//       int root_src = find(mfset, closest[j].src);
//       int root_dest = find(mfset, closest[j].dest);
//
//       if (root_src != root_dest) {
//         clone_edge(&closest[j], &mst->edges[mst_edges + local_mst_edges]);
//         local_mst_edges++;
//         unite(mfset, root_src, root_dest);
//       }
//     }
//   }
//
//   mst_edge_counts[thread_id] = local_mst_edges;
// }
//
// for(int j = 1; j < omp_get_max_threads(); j++) {
//   mst_edge_counts[j] += mst_edge_counts[j - 1];
// }
//
// mst_edges += mst_edge_counts[omp_get_max_threads() - 1];
// free(mst_edge_counts);
// #endif
