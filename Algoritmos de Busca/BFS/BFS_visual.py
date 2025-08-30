import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
from collections import deque
import time

# Configurar matplotlib para modo interativo
plt.ion()

class BFSVisualization:
    def __init__(self):
        # Definindo um grafo de exemplo simples
        # Formato: {vértice: [vizinhos]}
        self.graph = {
            'A': ['B', 'C'],
            'B': ['A', 'D', 'E'],
            'C': ['A', 'F'],
            'D': ['B'],
            'E': ['B', 'F'],
            'F': ['C', 'E']
        }
        
        # Vértices do grafo
        self.vertices = list(self.graph.keys())
        
        # Para visualização do algoritmo
        self.visited = set()
        self.queue = deque()
        self.current_vertex = None
        self.path = []
        self.fig = None
        self.pos = None
        
        # Para construção da árvore BFS
        self.tree_edges = []  # Arestas que formam a árvore BFS
        self.parent = {}      # Dicionário pai->filho para construir a árvore
        self.levels = {}      # Nível de cada vértice na árvore
        
    def show_initial_graph(self):
        """Mostra o grafo inicial antes de começar o algoritmo"""
        self.fig, (self.ax1, self.ax2, self.ax3) = plt.subplots(1, 3, figsize=(20, 8))
        
        # Cria o grafo NetworkX
        G = nx.Graph()
        for vertex, neighbors in self.graph.items():
            for neighbor in neighbors:
                G.add_edge(vertex, neighbor)
        
        # Posicionamento fixo dos vértices para toda a animação
        self.pos = nx.spring_layout(G, seed=42)
        
        # Subplot 1: Grafo original
        nx.draw_networkx_edges(G, self.pos, edge_color='blue', width=2, alpha=0.7, ax=self.ax1)
        nx.draw_networkx_nodes(G, self.pos, node_color='lightblue', 
                              node_size=1500, alpha=0.9, ax=self.ax1)
        nx.draw_networkx_labels(G, self.pos, font_size=16, font_weight='bold', ax=self.ax1)
        
        self.ax1.set_title("GRAFO ORIGINAL", fontsize=14, fontweight='bold', color='blue')
        self.ax1.axis('off')
        
        # Subplot 2: Estado atual do BFS
        self.ax2.set_title("EXPLORAÇÃO BFS", fontsize=14, fontweight='bold')
        self.ax2.axis('off')
        
        # Subplot 3: Árvore BFS
        self.ax3.set_title("ÁRVORE BFS", fontsize=14, fontweight='bold', color='green')
        self.ax3.axis('off')
        
        # Informações iniciais no subplot 2
        info_text = []
        info_text.append("📊 ALGORITMO BFS")
        info_text.append("=" * 20)
        info_text.append("")
        info_text.append("🔗 ADJACÊNCIAS:")
        for vertex, neighbors in self.graph.items():
            neighbors_str = ", ".join(neighbors)
            info_text.append(f"   {vertex}: {neighbors_str}")
        
        info_text.append("")
        info_text.append("� CONSTRUINDO ÁRVORE:")
        info_text.append("   • Cada vértice visitado")
        info_text.append("     vira um nó da árvore")
        info_text.append("   • Arestas de descoberta")
        info_text.append("     formam a estrutura")
        
        text_content = "\n".join(info_text)
        self.ax2.text(0.05, 0.95, text_content, transform=self.ax2.transAxes, fontsize=11,
                     verticalalignment='top', fontfamily='monospace',
                     bbox=dict(boxstyle='round,pad=0.5', facecolor='lightcyan', alpha=0.8))
        
        plt.tight_layout()
        plt.draw()
        plt.pause(3)  # Mostra por 3 segundos
    
    def bfs_algorithm(self, start_vertex='A'):
        """Algoritmo BFS com visualização passo a passo"""
        print("🌟 ALGORITMO BFS - BUSCA EM LARGURA")
        print("=" * 45)
        print("📊 Mostrando grafo inicial...")
        self.show_initial_graph()
        
        print("📊 Grafo original:")
        for vertex, neighbors in self.graph.items():
            neighbors_str = ", ".join(neighbors)
            print(f"   {vertex}: {neighbors_str}")
        
        print(f"\n🚀 Iniciando BFS do vértice: {start_vertex}")
        print("🔄 Executando algoritmo passo a passo:")
        print("-" * 45)
        
        # Inicialização
        self.visited = set()
        self.queue = deque([start_vertex])
        self.path = []
        self.tree_edges = []
        self.parent = {}
        self.levels = {start_vertex: 0}
        step = 0
        
        # Primeiro passo: adiciona o vértice inicial
        print(f"\nPasso {step}: Iniciando com vértice {start_vertex}")
        print(f"   📥 Fila: {list(self.queue)}")
        print(f"   ✅ Visitados: {sorted(self.visited)}")
        
        self.current_vertex = start_vertex
        self.visualize_step(f"Iniciando com vértice {start_vertex}", step)
        time.sleep(2)
        
        step += 1
        
        while self.queue:
            # Remove o primeiro vértice da fila
            current = self.queue.popleft()
            
            print(f"\nPasso {step}: Processando vértice {current}")
            
            if current not in self.visited:
                # Marca como visitado
                self.visited.add(current)
                self.path.append(current)
                
                print(f"   🎯 Vértice {current} visitado!")
                print(f"   📋 Caminho atual: {' → '.join(self.path)}")
                
                # Adiciona todos os vizinhos não visitados à fila
                neighbors_added = []
                for neighbor in self.graph[current]:
                    if neighbor not in self.visited and neighbor not in self.queue:
                        self.queue.append(neighbor)
                        neighbors_added.append(neighbor)
                        
                        # Constrói a árvore BFS
                        self.parent[neighbor] = current
                        self.tree_edges.append((current, neighbor))
                        self.levels[neighbor] = self.levels[current] + 1
                
                if neighbors_added:
                    neighbors_str = ", ".join(neighbors_added)
                    print(f"   ➕ Adicionados à fila: {neighbors_str}")
                
                print(f"   📥 Fila atual: {list(self.queue)}")
                print(f"   ✅ Visitados: {sorted(self.visited)}")
                
                # Atualiza visualização
                self.current_vertex = current
                self.visualize_step(f"Processando vértice {current}", step)
                
                step += 1
                time.sleep(2)
            else:
                print(f"   ⚠️  Vértice {current} já foi visitado")
        
        print(f"\n🎉 BFS CONCLUÍDO!")
        print(f"📋 Ordem de visitação: {' → '.join(self.path)}")
        print(f"✅ Total de vértices visitados: {len(self.visited)}")
        
        return self.path
    
    def visualize_step(self, title, step):
        """Visualiza o estado atual do algoritmo BFS com árvore"""
        # Limpa os subplots
        self.ax1.clear()
        self.ax2.clear()
        self.ax3.clear()
        
        # Cria o grafo NetworkX
        G = nx.Graph()
        for vertex, neighbors in self.graph.items():
            for neighbor in neighbors:
                G.add_edge(vertex, neighbor)
        
        # SUBPLOT 1: Grafo original com estado atual
        # Desenha todas as arestas em cinza claro
        nx.draw_networkx_edges(G, self.pos, edge_color='lightgray', width=1, alpha=0.5, ax=self.ax1)
        
        # Desenha arestas da árvore BFS em verde
        if self.tree_edges:
            tree_graph = nx.Graph()
            tree_graph.add_edges_from(self.tree_edges)
            nx.draw_networkx_edges(tree_graph, self.pos, edge_color='green', 
                                 width=3, ax=self.ax1)
        
        # Define cores dos vértices
        node_colors = []
        for vertex in self.vertices:
            if vertex == self.current_vertex:
                node_colors.append('red')  # Vértice sendo processado
            elif vertex in self.visited:
                node_colors.append('lightgreen')  # Vértices visitados
            elif vertex in self.queue:
                node_colors.append('orange')  # Vértices na fila
            else:
                node_colors.append('lightgray')  # Vértices não visitados
        
        # Desenha os vértices
        nx.draw_networkx_nodes(G, self.pos, node_color=node_colors, 
                              node_size=1200, alpha=0.9, ax=self.ax1)
        
        # Adiciona labels dos vértices
        nx.draw_networkx_labels(G, self.pos, font_size=14, font_weight='bold', ax=self.ax1)
        
        self.ax1.set_title(f"EXPLORAÇÃO BFS - {title}", fontsize=12, fontweight='bold')
        self.ax1.axis('off')
        
        # SUBPLOT 2: Informações do algoritmo
        self.ax2.axis('off')
        
        info_text = []
        info_text.append("ESTADO ATUAL DO BFS")
        info_text.append("=" * 25)
        info_text.append("")
        
        # Vértice atual
        if self.current_vertex:
            info_text.append(f"🎯 Processando: {self.current_vertex}")
            if self.current_vertex in self.levels:
                info_text.append(f"   Nível: {self.levels[self.current_vertex]}")
        info_text.append("")
        
        # Fila atual
        if self.queue:
            queue_str = " ← ".join(list(self.queue))
            info_text.append("📥 FILA (próximos):")
            info_text.append(f"   {queue_str}")
        else:
            info_text.append("📥 FILA: Vazia")
        info_text.append("")
        
        # Visitados por nível
        if self.levels:
            info_text.append("📊 NÍVEIS DA ÁRVORE:")
            max_level = max(self.levels.values()) if self.levels else 0
            for level in range(max_level + 1):
                vertices_at_level = [v for v, l in self.levels.items() if l == level and v in self.visited]
                if vertices_at_level:
                    info_text.append(f"   Nível {level}: {', '.join(sorted(vertices_at_level))}")
        
        info_text.append("")
        
        # Caminho
        if self.path:
            path_str = " → ".join(self.path)
            info_text.append("📋 ORDEM DE VISITA:")
            info_text.append(f"   {path_str}")
        
        text_content = "\n".join(info_text)
        self.ax2.text(0.05, 0.95, text_content, transform=self.ax2.transAxes, fontsize=10,
                     verticalalignment='top', fontfamily='monospace',
                     bbox=dict(boxstyle='round,pad=0.5', facecolor='lightyellow', alpha=0.8))
        
        # SUBPLOT 3: Árvore BFS
        self.draw_bfs_tree()
        
        plt.tight_layout()
        plt.draw()
        plt.pause(0.1)  # Pequena pausa para renderização
    
    def draw_bfs_tree(self):
        """Desenha a árvore BFS no subplot 3"""
        self.ax3.clear()
        self.ax3.set_title("ÁRVORE BFS", fontsize=12, fontweight='bold', color='green')
        self.ax3.axis('off')
        
        if not self.tree_edges:
            # Se não há árvore ainda, mostra mensagem
            self.ax3.text(0.5, 0.5, "Árvore será\nconstruída\nconforme BFS\nprogredir", 
                         transform=self.ax3.transAxes, fontsize=12, ha='center', va='center',
                         bbox=dict(boxstyle='round,pad=0.5', facecolor='lightgreen', alpha=0.5))
            return
        
        # Cria um grafo da árvore BFS apenas com nós visitados
        tree_graph = nx.Graph()
        
        # Adiciona apenas arestas que conectam nós já visitados
        for edge in self.tree_edges:
            if edge[0] in self.visited and edge[1] in self.visited:
                tree_graph.add_edge(edge[0], edge[1])
        
        # Adiciona nós visitados que podem estar isolados
        for vertex in self.visited:
            tree_graph.add_node(vertex)
        
        if len(tree_graph.nodes()) == 0:
            return
        
        # Calcula posições hierárquicas para a árvore
        tree_pos = self.calculate_tree_positions()
        
        # Filtra posições apenas para nós que existem no grafo
        filtered_tree_pos = {node: pos for node, pos in tree_pos.items() if node in tree_graph.nodes()}
        
        # Desenha as arestas da árvore
        if len(tree_graph.edges()) > 0 and len(filtered_tree_pos) > 1:
            nx.draw_networkx_edges(tree_graph, filtered_tree_pos, edge_color='green', 
                                 width=2, ax=self.ax3)
        
        # Define cores dos nós na árvore
        tree_node_colors = []
        for vertex in tree_graph.nodes():
            if vertex == self.current_vertex:
                tree_node_colors.append('red')
            elif vertex in self.visited:
                tree_node_colors.append('lightgreen')
            else:
                tree_node_colors.append('orange')
        
        # Desenha os nós da árvore
        if len(filtered_tree_pos) > 0:
            nx.draw_networkx_nodes(tree_graph, filtered_tree_pos, node_color=tree_node_colors, 
                                  node_size=800, alpha=0.9, ax=self.ax3)
            
            # Adiciona labels
            nx.draw_networkx_labels(tree_graph, filtered_tree_pos, font_size=12, font_weight='bold', ax=self.ax3)
        
        # Adiciona informações sobre os níveis
        if self.levels:
            visited_levels = {v: l for v, l in self.levels.items() if v in self.visited}
            if visited_levels:
                max_level = max(visited_levels.values())
                level_info = f"Profundidade: {max_level}\nNós: {len(self.visited)}"
                self.ax3.text(0.02, 0.98, level_info, transform=self.ax3.transAxes, fontsize=10,
                             verticalalignment='top',
                             bbox=dict(boxstyle='round,pad=0.3', facecolor='lightgreen', alpha=0.7))
    
    def calculate_tree_positions(self):
        """Calcula posições hierárquicas para desenhar a árvore"""
        if not self.levels:
            return {}
        
        # Organiza vértices por nível
        levels_dict = {}
        for vertex, level in self.levels.items():
            if vertex in self.visited:  # Só mostra vértices já visitados
                if level not in levels_dict:
                    levels_dict[level] = []
                levels_dict[level].append(vertex)
        
        # Calcula posições
        tree_pos = {}
        max_level = max(levels_dict.keys()) if levels_dict else 0
        
        for level, vertices in levels_dict.items():
            y = max_level - level  # Nível 0 no topo
            vertices.sort()  # Ordena para consistência
            
            if len(vertices) == 1:
                x = 0
                tree_pos[vertices[0]] = (x, y)
            else:
                # Distribui horizontalmente
                width = len(vertices) - 1
                for i, vertex in enumerate(vertices):
                    x = (i - width/2) * 1.5  # Espaçamento horizontal
                    tree_pos[vertex] = (x, y)
        
        return tree_pos
    
    def create_final_visualization(self, path):
        """Cria a visualização final do BFS com árvore completa"""
        # Limpa o conteúdo anterior
        self.ax1.clear()
        self.ax2.clear()
        self.ax3.clear()
        
        # SUBPLOT 1: Grafo original com árvore BFS
        G = nx.Graph()
        for vertex, neighbors in self.graph.items():
            for neighbor in neighbors:
                G.add_edge(vertex, neighbor)
        
        # Desenha todas as arestas em cinza claro
        nx.draw_networkx_edges(G, self.pos, edge_color='lightgray', width=1, alpha=0.3, ax=self.ax1)
        
        # Desenha a árvore BFS em verde
        if self.tree_edges:
            tree_graph = nx.Graph()
            tree_graph.add_edges_from(self.tree_edges)
            nx.draw_networkx_edges(tree_graph, self.pos, edge_color='green', 
                                 width=4, ax=self.ax1)
        
        # Desenha os vértices - todos visitados em verde
        nx.draw_networkx_nodes(G, self.pos, node_color='lightgreen', 
                              node_size=1500, alpha=0.9, ax=self.ax1)
        
        # Labels dos vértices
        nx.draw_networkx_labels(G, self.pos, font_size=16, font_weight='bold', ax=self.ax1)
        
        self.ax1.set_title(f"GRAFO COM ÁRVORE BFS\nOrdem: {' → '.join(path)}", 
                          fontsize=14, fontweight='bold', color='darkgreen')
        self.ax1.axis('off')
        
        # SUBPLOT 2: Resumo do algoritmo
        self.ax2.axis('off')
        
        summary_text = []
        summary_text.append("📊 RESUMO DO ALGORITMO BFS")
        summary_text.append("=" * 30)
        summary_text.append("")
        summary_text.append("🎯 OBJETIVO:")
        summary_text.append("   Explorar o grafo em largura,")
        summary_text.append("   construindo uma árvore de busca")
        summary_text.append("")
        summary_text.append("🌳 ÁRVORE BFS:")
        summary_text.append(f"   • {len(self.tree_edges)} arestas da árvore")
        if self.levels:
            max_level = max(self.levels.values())
            summary_text.append(f"   • Profundidade: {max_level}")
            summary_text.append("")
            summary_text.append("📊 NÍVEIS:")
            for level in range(max_level + 1):
                vertices_at_level = [v for v, l in self.levels.items() if l == level]
                if vertices_at_level:
                    summary_text.append(f"   Nível {level}: {', '.join(sorted(vertices_at_level))}")
        
        summary_text.append("")
        summary_text.append("🏆 RESULTADO:")
        summary_text.append(f"   • {len(path)} vértices visitados")
        summary_text.append(f"   • Ordem: {' → '.join(path)}")
        summary_text.append("")
        summary_text.append("💡 CARACTERÍSTICAS:")
        summary_text.append("   • Visita por níveis")
        summary_text.append("   • Menor distância da raiz")
        summary_text.append("   • Estrutura hierárquica")
        
        text_content = "\n".join(summary_text)
        self.ax2.text(0.05, 0.95, text_content, transform=self.ax2.transAxes, fontsize=10,
                     verticalalignment='top', fontfamily='monospace',
                     bbox=dict(boxstyle='round,pad=0.5', facecolor='lightblue', alpha=0.8))
        
        # SUBPLOT 3: Árvore BFS final
        self.ax3.clear()
        self.ax3.set_title("ÁRVORE BFS COMPLETA", fontsize=14, fontweight='bold', color='darkgreen')
        self.ax3.axis('off')
        
        # Desenha a árvore completa
        if self.tree_edges:
            tree_graph = nx.Graph()
            tree_graph.add_edges_from(self.tree_edges)
            
            # Adiciona todos os vértices visitados
            for vertex in self.visited:
                tree_graph.add_node(vertex)
            
            # Calcula posições hierárquicas
            tree_pos = self.calculate_tree_positions()
            
            # Desenha arestas da árvore
            nx.draw_networkx_edges(tree_graph, tree_pos, edge_color='darkgreen', 
                                 width=3, ax=self.ax3)
            
            # Desenha nós da árvore
            nx.draw_networkx_nodes(tree_graph, tree_pos, node_color='lightgreen', 
                                  node_size=1000, alpha=0.9, ax=self.ax3)
            
            # Labels
            nx.draw_networkx_labels(tree_graph, tree_pos, font_size=14, font_weight='bold', ax=self.ax3)
            
            # Adiciona informação sobre a raiz
            root = path[0] if path else 'A'
            self.ax3.text(0.5, 0.02, f"Raiz: {root}", transform=self.ax3.transAxes, 
                         fontsize=12, ha='center', fontweight='bold',
                         bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.7))
        
        plt.tight_layout()
        plt.draw()
        plt.pause(0.1)
        
        # Mantém a janela aberta
        print("🎯 Pressione Enter para finalizar...")
        input()
        plt.ioff()  # Desliga modo interativo
        plt.show()

def main():
    """Função principal para executar a demonstração"""
    print("🚀 Iniciando demonstração do Algoritmo BFS")
    print("⏳ Aguarde... A visualização será exibida passo a passo\n")
    
    bfs_demo = BFSVisualization()
    
    # Executa o algoritmo
    path = bfs_demo.bfs_algorithm(start_vertex='A')
    
    # Visualização final
    print(f"\n📊 Criando visualização final...")
    bfs_demo.create_final_visualization(path)
    
    print(f"\n✨ Demonstração concluída!")
    print(f"📈 O BFS explorou o grafo em largura, visitando todos os vértices")
    print(f"   em ordem de distância crescente do vértice inicial.")

if __name__ == "__main__":
    main()