import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
from matplotlib.animation import FuncAnimation
import time

# Configurar matplotlib para modo interativo
plt.ion()

class KruskalVisualization:
    def __init__(self):
        # Definindo um grafo de exemplo simples
        # Formato: (vértice1, vértice2, peso)
        self.edges = [
            ('A', 'B', 4),
            ('A', 'C', 2),
            ('B', 'C', 1),
            ('B', 'D', 5),
            ('C', 'D', 8),
            ('C', 'E', 10),
            ('D', 'E', 2),
            ('D', 'F', 6),
            ('E', 'F', 3)
        ]
        
        # Vértices únicos
        self.vertices = list(set([edge[0] for edge in self.edges] + [edge[1] for edge in self.edges]))
        
        # Estrutura Union-Find
        self.parent = {v: v for v in self.vertices}
        self.rank = {v: 0 for v in self.vertices}
        
        # Para visualização
        self.mst_edges = []
        self.current_edge = None
        self.step = 0
        self.fig = None
        self.pos = None
        
    def find(self, x):
        """Encontra o representante do conjunto com compressão de caminho"""
        if self.parent[x] != x:
            self.parent[x] = self.find(self.parent[x])
        return self.parent[x]
    
    def union(self, x, y):
        """Une dois conjuntos usando união por rank"""
        root_x = self.find(x)
        root_y = self.find(y)
        
        if root_x != root_y:
            if self.rank[root_x] < self.rank[root_y]:
                self.parent[root_x] = root_y
            elif self.rank[root_x] > self.rank[root_y]:
                self.parent[root_y] = root_x
            else:
                self.parent[root_y] = root_x
                self.rank[root_x] += 1
            return True
        return False
    
    def show_initial_graph(self):
        """Mostra o grafo inicial antes de começar o algoritmo"""
        self.fig = plt.figure(figsize=(12, 8))
        
        # Cria o grafo
        G = nx.Graph()
        for edge in self.edges:
            G.add_edge(edge[0], edge[1], weight=edge[2])
        
        # Posicionamento dos vértices (fixo para toda a animação)
        self.pos = nx.spring_layout(G, seed=42)
        
        # Desenha todas as arestas
        nx.draw_networkx_edges(G, self.pos, edge_color='blue', width=2, alpha=0.7)
        
        # Desenha os vértices
        nx.draw_networkx_nodes(G, self.pos, node_color='lightblue', 
                              node_size=1000, alpha=0.9)
        
        # Adiciona labels dos vértices
        nx.draw_networkx_labels(G, self.pos, font_size=16, font_weight='bold')
        
        # Adiciona pesos das arestas
        edge_labels = {(edge[0], edge[1]): edge[2] for edge in self.edges}
        nx.draw_networkx_edge_labels(G, self.pos, edge_labels, font_size=10)
        
        plt.title("GRAFO INICIAL - Algoritmo de Kruskal\nTodas as arestas com seus pesos", 
                 fontsize=16, fontweight='bold', color='blue')
        
        plt.axis('off')
        plt.tight_layout()
        
        # Mostra o grafo inicial
        plt.draw()
        plt.pause(3)  # Mostra por 3 segundos
    def kruskal_algorithm(self):
        """Algoritmo de Kruskal com visualização passo a passo"""
        # Mostra o grafo inicial
        print("🌟 ALGORITMO DE KRUSKAL - ÁRVORE GERADORA MÍNIMA")
        print("=" * 55)
        print("📊 Mostrando grafo inicial...")
        self.show_initial_graph()
        
        # Ordena as arestas por peso
        sorted_edges = sorted(self.edges, key=lambda x: x[2])
        
        print("📊 Grafo original:")
        for edge in self.edges:
            print(f"   {edge[0]} -- {edge[1]} (peso: {edge[2]})")
        
        print(f"\n📈 Arestas ordenadas por peso:")
        for edge in sorted_edges:
            print(f"   {edge[0]} -- {edge[1]} (peso: {edge[2]})")
        
        print(f"\n🔄 Executando algoritmo passo a passo:")
        print("-" * 40)
        
        mst = []
        total_weight = 0
        
        for i, (u, v, weight) in enumerate(sorted_edges):
            print(f"\nPasso {i+1}: Analisando aresta {u} -- {v} (peso: {weight})")
            
            # Verifica se os vértices estão no mesmo componente
            root_u = self.find(u)
            root_v = self.find(v)
            
            print(f"   Componente de {u}: {root_u}")
            print(f"   Componente de {v}: {root_v}")
            
            if root_u != root_v:
                # Não forma ciclo, adiciona à MST
                self.union(u, v)
                mst.append((u, v, weight))
                total_weight += weight
                print(f"   ✅ Aresta adicionada à MST! Peso total: {total_weight}")
                
                # Atualiza para visualização
                self.mst_edges = mst.copy()
                self.current_edge = (u, v, weight)
                self.visualize_step(sorted_edges, i)
                
            else:
                print(f"   ❌ Aresta rejeitada (formaria ciclo)")
                
                # Atualiza visualização mesmo para arestas rejeitadas
                self.current_edge = (u, v, weight)
                self.visualize_step(sorted_edges, i)
                
            # Pausa para visualização
            time.sleep(2)
        
        print(f"\n🎉 RESULTADO FINAL:")
        print(f"📋 Árvore Geradora Mínima:")
        for edge in mst:
            print(f"   {edge[0]} -- {edge[1]} (peso: {edge[2]})")
        print(f"⚖️  Peso total da MST: {total_weight}")
        
        return mst, total_weight
    
    def visualize_step(self, sorted_edges, current_step):
        """Visualiza o estado atual do algoritmo"""
        # Inicializa a figura apenas uma vez
        if self.fig is None:
            self.fig = plt.figure(figsize=(12, 8))
            # Cria o grafo para posicionamento fixo
            G = nx.Graph()
            for edge in self.edges:
                G.add_edge(edge[0], edge[1], weight=edge[2])
            self.pos = nx.spring_layout(G, seed=42)
        
        # Limpa o conteúdo anterior
        plt.clf()
        
        # Cria o grafo
        G = nx.Graph()
        for edge in self.edges:
            G.add_edge(edge[0], edge[1], weight=edge[2])
        
        # Desenha todas as arestas em cinza claro
        nx.draw_networkx_edges(G, self.pos, edge_color='lightgray', width=1, alpha=0.5)
        
        # Desenha as arestas da MST em verde
        mst_graph = nx.Graph()
        for edge in self.mst_edges:
            mst_graph.add_edge(edge[0], edge[1])
        
        if len(self.mst_edges) > 0:
            nx.draw_networkx_edges(mst_graph, self.pos, edge_color='green', width=3)
        
        # Destaca a aresta atual sendo analisada
        if self.current_edge:
            current_graph = nx.Graph()
            current_graph.add_edge(self.current_edge[0], self.current_edge[1])
            nx.draw_networkx_edges(current_graph, self.pos, edge_color='red', width=2, style='dashed')
        
        # Desenha os vértices
        nx.draw_networkx_nodes(G, self.pos, node_color='lightblue', 
                              node_size=1000, alpha=0.9)
        
        # Adiciona labels dos vértices
        nx.draw_networkx_labels(G, self.pos, font_size=16, font_weight='bold')
        
        # Adiciona pesos das arestas
        edge_labels = {(edge[0], edge[1]): edge[2] for edge in self.edges}
        nx.draw_networkx_edge_labels(G, self.pos, edge_labels, font_size=10)
        
        # Título e informações
        current_edge_info = ""
        if self.current_edge:
            status = "ADICIONADA" if any(self.current_edge[:2] == (e[0], e[1]) or self.current_edge[:2] == (e[1], e[0]) for e in self.mst_edges) else "REJEITADA"
            current_edge_info = f"Aresta atual: {self.current_edge[0]}-{self.current_edge[1]} ({self.current_edge[2]}) - {status}"
        
        plt.title(f"Algoritmo de Kruskal - Passo {current_step + 1}\n{current_edge_info}", 
                 fontsize=14, fontweight='bold')
        
        # Legenda
        from matplotlib.lines import Line2D
        legend_elements = [
            Line2D([0], [0], color='lightgray', lw=1, alpha=0.5, label='Arestas originais'),
            Line2D([0], [0], color='green', lw=3, label='MST (arestas aceitas)'),
            Line2D([0], [0], color='red', lw=2, linestyle='--', label='Aresta sendo analisada')
        ]
        plt.legend(handles=legend_elements, loc='upper right')
        
        plt.axis('off')
        plt.tight_layout()
        
        # Atualiza a visualização
        plt.draw()
        plt.pause(0.1)  # Pequena pausa para renderização
        
    def create_final_visualization(self, mst, total_weight):
        """Cria a visualização final da MST"""
        # Limpa o conteúdo anterior se existir figura
        if self.fig is not None:
            plt.clf()
        else:
            self.fig = plt.figure(figsize=(12, 8))
        
        # Cria o grafo original
        G = nx.Graph()
        for edge in self.edges:
            G.add_edge(edge[0], edge[1], weight=edge[2])
        
        # Usa o mesmo posicionamento se já foi calculado
        if self.pos is None:
            self.pos = nx.spring_layout(G, seed=42)
        
        # Desenha todas as arestas em cinza claro
        nx.draw_networkx_edges(G, self.pos, edge_color='lightgray', width=1, alpha=0.3)
        
        # Desenha as arestas da MST
        mst_graph = nx.Graph()
        for edge in mst:
            mst_graph.add_edge(edge[0], edge[1])
        
        nx.draw_networkx_edges(mst_graph, self.pos, edge_color='green', width=4)
        
        # Desenha os vértices
        nx.draw_networkx_nodes(G, self.pos, node_color='lightgreen', 
                              node_size=1200, alpha=0.9)
        
        # Adiciona labels dos vértices
        nx.draw_networkx_labels(G, self.pos, font_size=16, font_weight='bold')
        
        # Adiciona pesos apenas das arestas da MST
        mst_edge_labels = {(edge[0], edge[1]): edge[2] for edge in mst}
        nx.draw_networkx_edge_labels(mst_graph, self.pos, mst_edge_labels, 
                                   font_size=12, font_weight='bold', 
                                   bbox=dict(boxstyle='round,pad=0.3', 
                                           facecolor='yellow', alpha=0.8))
        
        plt.title(f"ÁRVORE GERADORA MÍNIMA - Algoritmo de Kruskal\nPeso Total: {total_weight}", 
                 fontsize=16, fontweight='bold', color='darkgreen')
        
        plt.axis('off')
        plt.tight_layout()
        
        # Atualiza a visualização final
        plt.draw()
        plt.pause(0.1)
        
        # Mantém a janela aberta
        print("🎯 Pressione Enter para finalizar...")
        input()
        plt.ioff()  # Desliga modo interativo
        plt.show()

def main():
    """Função principal para executar a demonstração"""
    print("🚀 Iniciando demonstração do Algoritmo de Kruskal")
    print("⏳ Aguarde... A visualização será exibida passo a passo\n")
    
    kruskal_demo = KruskalVisualization()
    
    # Executa o algoritmo
    mst, total_weight = kruskal_demo.kruskal_algorithm()
    
    # Visualização final
    print(f"\n📊 Criando visualização final...")
    kruskal_demo.create_final_visualization(mst, total_weight)
    
    print(f"\n✨ Demonstração concluída!")
    print(f"📈 A Árvore Geradora Mínima conecta todos os vértices com o menor peso total possível.")

if __name__ == "__main__":
    main()