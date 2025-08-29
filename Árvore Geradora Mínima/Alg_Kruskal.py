import networkx as nx
import matplotlib.pyplot as plt
import time

def kruskal_visual():
    # Criando o grafo com pesos
    G = nx.Graph()
    edges = [
        ('A', 'B', 7),
        ('A', 'D', 5),
        ('B', 'C', 8),
        ('B', 'D', 9),
        ('B', 'E', 7),
        ('C', 'E', 5),
        ('D', 'E', 15),
        ('D', 'F', 6),
        ('E', 'F', 8),
        ('E', 'G', 9),
        ('F', 'G', 11)
    ]
    G.add_weighted_edges_from(edges)

    pos = nx.spring_layout(G, seed=42)  # Layout fixo

    # Ordenando as arestas pelo peso
    sorted_edges = sorted(G.edges(data=True), key=lambda x: x[2]['weight'])

    parent = {}
    rank = {}

    def find(node):
        if parent[node] != node:
            parent[node] = find(parent[node])
        return parent[node]

    def union(node1, node2):
        root1, root2 = find(node1), find(node2)
        if root1 != root2:
            if rank[root1] < rank[root2]:
                parent[root1] = root2
            elif rank[root1] > rank[root2]:
                parent[root2] = root1
            else:
                parent[root2] = root1
                rank[root1] += 1

    # Inicializa conjuntos
    for node in G.nodes():
        parent[node] = node
        rank[node] = 0

    mst_edges = []

    plt.ion()  # Ativar modo interativo
    fig, ax = plt.subplots(figsize=(8, 6))

    for u, v, data in sorted_edges:
        plt.cla()
        nx.draw(G, pos, with_labels=True, node_color='lightblue', node_size=600, ax=ax)
        nx.draw_networkx_edge_labels(G, pos, edge_labels={(a,b):d['weight'] for a,b,d in G.edges(data=True)}, ax=ax)
        nx.draw_networkx_edges(G, pos, edgelist=mst_edges, width=3, edge_color='green', ax=ax)

        if find(u) != find(v):
            mst_edges.append((u, v))
            union(u, v)

        plt.title(f"Adicionando aresta: {u}-{v} (peso {data['weight']})")
        plt.pause(1.5)

    plt.title("Árvore Geradora Mínima Concluída (Kruskal)")
    plt.ioff()
    plt.show()

if __name__ == "__main__":
    kruskal_visual()
