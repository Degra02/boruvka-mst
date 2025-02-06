#include "graph.h"
#include <mpi.h>
#include <time.h>

#ifndef UTILS_H
#define UTILS_H


#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_GREY "\x1b[90m"

/*
 * Print a debug message.
 * @param msg Message to print.
 * @param color Color of the message.
 * @param time Time of the message.
 * @param rank Rank of the process.
 * @params ... Additional parameters to format the message.
 */
// void print_debug(const char *, const char *, const int);
void debug(const char *, const char *, const int, ...);

/*
 * Get a random number.
 * @return Random number.
 * @note This function has to be tested to check if it provides a "more random" number than rand().
 */
unsigned int get_random();

/*
 * Generate a complete graph with V vertices, without saving it to a file.
 * @param V Number of vertices.
 * @param g Pointer to the adjacency graph.
 */
AG* generate_graph(const int);

/*
 * Broadcasts an adjacency graph.
 * @param g Pointer to the adjacency graph.
 */
void Bcast_adj_graph(AG **, MPI_Comm);


#endif // !UTILS_H
#define UTILS_H
