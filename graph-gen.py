import networkx as nx
import random
import sys


def generate_connected_graph(num_nodes, num_edges):
    while True:
        G = nx.gnm_random_graph(num_nodes, num_edges)
        if nx.is_connected(G):
            break
    # Assign random weights to edges
    for (u, v) in G.edges():
        G[u][v]['weight'] = random.randint(1, 10)  # Adjust weight range as needed
    return G



if __name__ == "__main__":
    num_nodes = int(sys.argv[1]) 
    num_edges = int(sys.argv[2])

    G = generate_connected_graph(num_nodes, num_edges)

    with open("graph.txt", "wb") as f:
        f.write(f"{num_nodes} {num_edges}\n".encode())
        nx.write_weighted_edgelist(G, f)


    

