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
 * Clones the edge `e`.
 * @param `to`: pointer to the destination edge
 * @param `from`: pointer to the source edge
*/
void clone_edge(Edge*, Edge*);

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
 * Initializes an adjacency list graph from a file.
 * @param `filename`: path to the file
 * @return `AG*`: pointer to the graph
 * @note The file must have the following format:
 *      - The first line contains the number of vertices `V` and the number of edges `E`
 *      - The following `E` lines contain the source vertex, destination vertex, and weight of each edge
*/
AG* init_from_file(const char*);


/*
 * Prints the adjacency list graph `g` to the specified file.
 * @param `g`: pointer to the graph
 * @param `filename`: path to the file
*/
void print_file_adj_graph(AG*, const char*);


/*
 * Prints the mst result to the specified file.
 * @param `mst`: pointer to the mst
 * @param `filename`: path to the file
*/
void print_file_mst(AG*, const char*);



/*
* Graph in CSR (Compressed Sparse Row) format.
* @attr `dest`: array of size `|E|` mapping each edge to its destination vertex
* @attr `w`: array of size `|E|` mapping each edge to its weight
* @attr `fe`: array of size `|V|` mapping each vertex to its first edge
* @attr `od`: array of size `|V|` mapping each vertex to its out degree
*/
typedef struct CSR_Graph {
  int V, E;
  int *dest;
  int *w;
  int *fe;
  int *od;
} CSRG;

/*
  * Initializes a graph with `V` vertices and `E` edges.
  * @param `V`: number of vertices
  * @param `E`: number of edges
  * @return `G*`: pointer to the graph
*/
CSRG* init_csr_graph(const int, const int);

/*
  * Frees the memory allocated for the graph.
  * @param `g`: pointer to the graph
*/
void free_csr_graph(CSRG*);

/*
  * Clones the graph `g`.
  * @param `g`: pointer to the graph
  * @return `G*`: pointer to the cloned graph
*/
CSRG* clone_csr_graph(CSRG*);


#endif // !GRAPH_H
