import networkx as nx
import random
import sys


def gen_graph(V, E):
    # while True:
    #     G = nx.gnm_random_graph(V, E)
    #     if nx.is_connected(G):
    #         break
        
    # G = nx.erdos_renyi_graph(V, 0.8, directed=False)
    G = nx.fast_gnp_random_graph(V, 0.8, directed=False)

    for (u, v) in G.edges():
        G[u][v]['weight'] = random.randint(1, 500)
    return G



if __name__ == "__main__":
    V = int(sys.argv[1])
    E = int(sys.argv[2])

    G = gen_graph(V, E)

    with open("graph.txt", "wb") as f:
        f.write(f"{V} {E}\n".encode())
        nx.write_weighted_edgelist(G, f)




