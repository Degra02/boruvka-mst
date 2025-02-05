#include <limits.h>
#include <stdlib.h>
#include <mpi.h>

#include "../include/mfset.h"
#include "../include/mst.h"
#include "../include/utils.h"

#ifdef _OPENMP
#include <omp.h>
#endif

void adj_boruvka(AG *g, AG *mst) {
  int rank;
  int size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int V, E;
  V = g->V;
  E = g->E;

  // split edges among processes
  int edges_per_proc = (E + size - 1) / size;
  Edge *local_edges;
  //local_edges = (Edge *)malloc(edges_per_proc * sizeof(Edge)); 
  MPI_Alloc_mem(edges_per_proc * 3 * sizeof(int), MPI_INFO_NULL, &local_edges);

  // transform the scatter into many scatters of 2GB chunks
  // int edges_per_chunk = 2 * 1024 * 1024 * 1024 / (3 * sizeof(int));
  // int chunks = (edges_per_proc + edges_per_chunk - 1) / edges_per_chunk;
  // int chunk_size = edges_per_chunk * 3;
  //
  // for (int i = 0; i < chunks; i++) {
  //   int chunk_edges = edges_per_chunk;
  //   if (i == chunks - 1) {
  //     chunk_edges = edges_per_proc - i * edges_per_chunk;
  //   }
  //
  //   MPI_Scatter(g->edges + i * edges_per_chunk, chunk_size, MPI_INT,
  //               local_edges + i * edges_per_chunk, chunk_size, MPI_INT, 0,
  //               MPI_COMM_WORLD);
  // }
  
  debug("Starting scatter of edges", ANSI_COLOR_CYAN, rank);
  MPI_Scatter(g->edges, edges_per_proc * 3, MPI_INT, local_edges, edges_per_proc * 3, MPI_INT, 0, MPI_COMM_WORLD);
  debug("Finished scatter of edges", ANSI_COLOR_CYAN, rank);

  if (rank == size - 1 && E % edges_per_proc != 0) {
    edges_per_proc = E % edges_per_proc;
  }
  // debug("edges_per_proc = %d", ANSI_COLOR_CYAN, rank, edges_per_proc);

  // TODO: check if the size is compatible with the number of processes

  // HACK: should MPI_Alloc_mem be used?
  MFSet *mfset = init_mfset(V);


  int mst_edges = 0;
  Edge *closest;
  Edge *closest_local;
  // closest = (Edge *)malloc(V * sizeof(Edge));
  // closest_local = (Edge *)malloc(V * sizeof(Edge));

  MPI_Alloc_mem(V * 3 * sizeof(int), MPI_INFO_NULL, &closest);
  MPI_Alloc_mem(V * 3 * sizeof(int), MPI_INFO_NULL, &closest_local);


  for (int i = 1; i < V && mst_edges < V - 1; i *= 2) {
    #pragma omp parallel for
    for (int j = 0; j < V; j++) {
      closest[j].w = INT_MAX;
    }

    // search for the closest edge
    #pragma omp parallel for
    for (int j = 0; j < edges_per_proc; j++) {
      Edge e = local_edges[j];

      int root_src = find(mfset, e.src);
      int root_dest = find(mfset, e.dest);

      if (root_src == root_dest) {
        continue;
      }

      if (e.w < closest[root_src].w) {
        #pragma omp critical
        clone_edge(&e, &closest[root_src]);
      }

      if (e.w < closest[root_dest].w) {
        #pragma omp critical
        clone_edge(&e, &closest[root_dest]);
      }
    }

    int from, to;
    // MPI_Request request;
    for (int st = 1; st < size; st *= 2) {
      if (rank % (2 * st) == 0) {
        from = rank + st;
        if (from < size) {
          MPI_Recv(closest_local, V * 3, MPI_INT, from, 0, MPI_COMM_WORLD,
                   MPI_STATUS_IGNORE);
          // MPI_Irecv(closest_local, V * 3, MPI_INT, from, 0, MPI_COMM_WORLD,
          //           &request);
          // MPI_Wait(&request, MPI_STATUS_IGNORE);

          // static scheduling seems to be the best option, as the workload 
          // is the same for each iteration
          #pragma omp parallel for schedule(static)
          for (int j = 0; j < V; j++) {
            if (closest_local[j].w < closest[j].w) {
              #pragma omp critical
              clone_edge(&closest_local[j], &closest[j]);
            }
          }
        }
      } else if (rank % st == 0) {
        to = rank - st;
        MPI_Send(closest, V * 3, MPI_INT, to, 0, MPI_COMM_WORLD);
        // MPI_Isend(closest, V * 3, MPI_INT, to, 0, MPI_COMM_WORLD, &request);
      }
    }

    MPI_Bcast(closest, V * 3, MPI_INT, 0, MPI_COMM_WORLD);

    for (int j = 0; j < V; j++) {
      if (closest[j].w != INT_MAX) {
        {
          int root_src = find(mfset, closest[j].src);
          int root_dest = find(mfset, closest[j].dest);

          if (root_src != root_dest) {
            clone_edge(&closest[j], &mst->edges[mst_edges]);
            mst_edges++;
            unite(mfset, root_src, root_dest);
          }
        }
      }
    } 
  }

  MPI_Free_mem(local_edges);
  MPI_Free_mem(closest);
  MPI_Free_mem(closest_local);

  free_mfset(mfset);
  // free(closest);
  // free(closest_local);
  // free(local_edges);
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

