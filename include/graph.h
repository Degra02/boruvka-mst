/* File containing the graph structure and the functions to manipulate it */

#ifndef GRAPH_H
#define GRAPH_H

/*
 * Graph in adjacency matrix format.
 * @attr `V`: number of vertices
 * @attr `E`: number of edges
 * @attr `adj`: 2D array of size `|V| x |V|` mapping each edge to its weight
*/
typedef struct M_Graph {
  int V, E;
  int **adj;
} MG;


/*
 * Edge structure.
 * @attr `src`: source vertex
 * @attr `dest`: destination vertex
 * @attr `w`: weight of the edge
*/
typedef struct Edge {
  int src, dest, w;
} Edge;

/*
 * Graph in adjacency list format.
 * @attr `V`: number of vertices
 * @attr `E`: number of edges
 * @attr `edges`: array of size `|E|` containing all the edges
*/
typedef struct Adj_Graph {
  int V, E;
  Edge *edges;
} AG;

/*
 * Initializes a graph with `V` vertices and `E` edges.
 * @param `V`: number of vertices
 * @param `E`: number of edges
 * @return `MG*`: pointer to the graph
*/
AG* init_adj_graph(const int, const int);

/*
 * Frees the memory allocated for the graph.
 * @param `g`: pointer to the graph
*/
void free_adj_graph(AG*);


/*
* Graph in CSR (Compressed Sparse Row) format.
* @attr `dest`: array of size `|E|` mapping each edge to its destination vertex
* @attr `w`: array of size `|E|` mapping each edge to its weight
* @attr `fe`: array of size `|V|` mapping each vertex to its first edge
* @attr `od`: array of size `|V|` mapping each vertex to its out degree
*/
typedef struct Graph {
  int V, E;
  int *dest;
  int *w;
  int *fe;
  int *od;
} G;

/*
  * Initializes a graph with `V` vertices and `E` edges.
  * @param `V`: number of vertices
  * @param `E`: number of edges
  * @return `G*`: pointer to the graph
*/
G* init_graph(const int, const int);

/*
  * Frees the memory allocated for the graph.
  * @param `g`: pointer to the graph
*/
void free_graph(G*);

/*
  * Clones the graph `g`.
  * @param `g`: pointer to the graph
  * @return `G*`: pointer to the cloned graph
*/
G* clone_graph(G*);


#endif // !GRAPH_H
