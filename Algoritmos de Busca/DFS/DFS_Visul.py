import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import time

# Configurar matplotlib para modo interativo
plt.ion()

class DFSVisualization:
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
        self.stack = []
        self.current_vertex = None
        self.path = []
        self.fig = None
        self.pos = None
        
        # Para construção da árvore DFS
        self.tree_edges = []  # Arestas que formam a árvore DFS
        self.parent = {}      # Dicionário pai->filho para construir a árvore
        self.discovery_time = {}  # Tempo de descoberta de cada vértice
        self.finish_time = {}     # Tempo de finalização de cada vértice
        self.time_counter = 0
        
        # Para mostrar o backtracking
        self.backtrack_edges = []  # Arestas usadas no backtracking
        
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
        
        # Subplot 2: Estado atual do DFS
        self.ax2.set_title("EXPLORAÇÃO DFS", fontsize=14, fontweight='bold')
        self.ax2.axis('off')
        
        # Subplot 3: Árvore DFS
        self.ax3.set_title("ÁRVORE DFS", fontsize=14, fontweight='bold', color='green')
        self.ax3.axis('off')
        
        # Informações iniciais no subplot 2
        info_text = []
        info_text.append("📊 ALGORITMO DFS")
        info_text.append("=" * 20)
        info_text.append("")
        info_text.append("🔗 ADJACÊNCIAS:")
        for vertex, neighbors in self.graph.items():
            neighbors_str = ", ".join(neighbors)
            info_text.append(f"   {vertex}: {neighbors_str}")
        
        info_text.append("")
        info_text.append("🌳 CONSTRUINDO ÁRVORE:")
        info_text.append("   • Explora em profundidade")
        info_text.append("   • Usa pilha (LIFO)")
        info_text.append("   • Vai o mais fundo possível")
        info_text.append("   • Faz backtracking")
        
        text_content = "\n".join(info_text)
        self.ax2.text(0.05, 0.95, text_content, transform=self.ax2.transAxes, fontsize=11,
                     verticalalignment='top', fontfamily='monospace',
                     bbox=dict(boxstyle='round,pad=0.5', facecolor='lightcyan', alpha=0.8))
        
        plt.tight_layout()
        plt.draw()
        plt.pause(3)  # Mostra por 3 segundos
    
    def dfs_algorithm(self, start_vertex='A'):
        """Algoritmo DFS com visualização passo a passo"""
        print("🌟 ALGORITMO DFS - BUSCA EM PROFUNDIDADE")
        print("=" * 50)
        print("📊 Mostrando grafo inicial...")
        self.show_initial_graph()
        
        print("📊 Grafo original:")
        for vertex, neighbors in self.graph.items():
            neighbors_str = ", ".join(neighbors)
            print(f"   {vertex}: {neighbors_str}")
        
        print(f"\n🚀 Iniciando DFS do vértice: {start_vertex}")
        print("🔄 Executando algoritmo passo a passo:")
        print("-" * 50)
        
        # Inicialização
        self.visited = set()
        self.stack = [start_vertex]
        self.path = []
        self.tree_edges = []
        self.parent = {}
        self.discovery_time = {}
        self.finish_time = {}
        self.time_counter = 0
        self.backtrack_edges = []
        step = 0
        
        # Primeiro passo: adiciona o vértice inicial
        print(f"\nPasso {step}: Iniciando com vértice {start_vertex}")
        print(f"   📚 Pilha: {self.stack}")
        print(f"   ✅ Visitados: {sorted(self.visited)}")
        
        self.current_vertex = start_vertex
        self.visualize_step(f"Iniciando com vértice {start_vertex}", step)
        time.sleep(2)
        
        step += 1
        
        while self.stack:
            # Pega o topo da pilha (mas não remove ainda)
            current = self.stack[-1]
            
            print(f"\nPasso {step}: Analisando vértice {current}")
            
            if current not in self.visited:
                # Marca como visitado
                self.visited.add(current)
                self.path.append(current)
                self.discovery_time[current] = self.time_counter
                self.time_counter += 1
                
                print(f"   🎯 Vértice {current} visitado!")
                print(f"   📋 Caminho atual: {' → '.join(self.path)}")
                print(f"   ⏰ Tempo de descoberta: {self.discovery_time[current]}")
                
                # Encontra vizinhos não visitados
                unvisited_neighbors = []
                for neighbor in self.graph[current]:
                    if neighbor not in self.visited:
                        unvisited_neighbors.append(neighbor)
                
                if unvisited_neighbors:
                    # Adiciona vizinhos à pilha (ordem reversa para manter ordem alfabética)
                    neighbors_added = []
                    for neighbor in reversed(sorted(unvisited_neighbors)):
                        if neighbor not in self.stack:
                            self.stack.append(neighbor)
                            neighbors_added.append(neighbor)
                            
                            # Constrói a árvore DFS
                            self.parent[neighbor] = current
                            self.tree_edges.append((current, neighbor))
                    
                    if neighbors_added:
                        neighbors_str = ", ".join(reversed(neighbors_added))
                        print(f"   ➕ Adicionados à pilha: {neighbors_str}")
                else:
                    print(f"   🔚 Sem vizinhos não visitados")
                
                print(f"   📚 Pilha atual: {self.stack}")
                print(f"   ✅ Visitados: {sorted(self.visited)}")
                
                # Atualiza visualização
                self.current_vertex = current
                self.visualize_step(f"Processando vértice {current}", step)
                
                step += 1
                time.sleep(2)
            else:
                # Vértice já visitado, faz backtracking
                finished_vertex = self.stack.pop()
                self.finish_time[finished_vertex] = self.time_counter
                self.time_counter += 1
                
                print(f"   🔙 Backtrack de {finished_vertex}")
                print(f"   ⏰ Tempo de finalização: {self.finish_time[finished_vertex]}")
                print(f"   📚 Pilha após backtrack: {self.stack}")
                
                # Adiciona aresta de backtrack para visualização
                if self.stack and finished_vertex in self.parent:
                    parent_vertex = self.parent[finished_vertex]
                    if parent_vertex in self.stack:
                        self.backtrack_edges.append((finished_vertex, parent_vertex))
                
                # Atualiza visualização
                if self.stack:
                    self.current_vertex = self.stack[-1]
                else:
                    self.current_vertex = None
                    
                self.visualize_step(f"Backtrack de {finished_vertex}", step)
                
                step += 1
                time.sleep(1.5)
        
        print(f"\n🎉 DFS CONCLUÍDO!")
        print(f"📋 Ordem de visitação: {' → '.join(self.path)}")
        print(f"✅ Total de vértices visitados: {len(self.visited)}")
        
        # Mostra tempos de descoberta e finalização
        print(f"\n⏰ TEMPOS:")
        for vertex in self.path:
            discovery = self.discovery_time.get(vertex, 'N/A')
            finish = self.finish_time.get(vertex, 'N/A')
            print(f"   {vertex}: descoberta={discovery}, finalização={finish}")
        
        return self.path
    
    def visualize_step(self, title, step):
        """Visualiza o estado atual do algoritmo DFS com árvore"""
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
        
        # Desenha arestas da árvore DFS em verde
        if self.tree_edges:
            tree_graph = nx.Graph()
            for edge in self.tree_edges:
                if edge[0] in self.visited and edge[1] in self.visited:
                    tree_graph.add_edge(edge[0], edge[1])
            
            if len(tree_graph.edges()) > 0:
                nx.draw_networkx_edges(tree_graph, self.pos, edge_color='green', 
                                     width=3, ax=self.ax1)
        
        # Desenha arestas de backtrack em vermelho pontilhado
        if self.backtrack_edges:
            backtrack_graph = nx.Graph()
            backtrack_graph.add_edges_from(self.backtrack_edges)
            nx.draw_networkx_edges(backtrack_graph, self.pos, edge_color='red', 
                                 width=2, style='dashed', alpha=0.7, ax=self.ax1)
        
        # Define cores dos vértices
        node_colors = []
        for vertex in self.vertices:
            if vertex == self.current_vertex:
                node_colors.append('red')  # Vértice sendo processado
            elif vertex in self.visited:
                node_colors.append('lightgreen')  # Vértices visitados
            elif vertex in self.stack:
                node_colors.append('orange')  # Vértices na pilha
            else:
                node_colors.append('lightgray')  # Vértices não visitados
        
        # Desenha os vértices
        nx.draw_networkx_nodes(G, self.pos, node_color=node_colors, 
                              node_size=1200, alpha=0.9, ax=self.ax1)
        
        # Adiciona labels dos vértices com tempos
        labels = {}
        for vertex in self.vertices:
            if vertex in self.discovery_time and vertex in self.finish_time:
                labels[vertex] = f"{vertex}\n{self.discovery_time[vertex]}/{self.finish_time[vertex]}"
            elif vertex in self.discovery_time:
                labels[vertex] = f"{vertex}\n{self.discovery_time[vertex]}/-"
            else:
                labels[vertex] = vertex
        
        nx.draw_networkx_labels(G, self.pos, labels, font_size=10, font_weight='bold', ax=self.ax1)
        
        self.ax1.set_title(f"EXPLORAÇÃO DFS - {title}", fontsize=12, fontweight='bold')
        self.ax1.axis('off')
        
        # Legenda
        from matplotlib.lines import Line2D
        legend_elements = [
            Line2D([0], [0], marker='o', color='w', markerfacecolor='red', 
                   markersize=15, label='Vértice atual'),
            Line2D([0], [0], marker='o', color='w', markerfacecolor='lightgreen', 
                   markersize=15, label='Visitados'),
            Line2D([0], [0], marker='o', color='w', markerfacecolor='orange', 
                   markersize=15, label='Na pilha'),
            Line2D([0], [0], marker='o', color='w', markerfacecolor='lightgray', 
                   markersize=15, label='Não visitados'),
            Line2D([0], [0], color='green', lw=3, label='Árvore DFS'),
            Line2D([0], [0], color='red', lw=2, linestyle='--', label='Backtrack')
        ]
        self.ax1.legend(handles=legend_elements, loc='upper right', fontsize=9)
        
        # SUBPLOT 2: Informações do algoritmo
        self.ax2.axis('off')
        
        info_text = []
        info_text.append("ESTADO ATUAL DO DFS")
        info_text.append("=" * 25)
        info_text.append("")
        
        # Vértice atual
        if self.current_vertex:
            info_text.append(f"🎯 Processando: {self.current_vertex}")
            if self.current_vertex in self.discovery_time:
                info_text.append(f"   Descoberta: {self.discovery_time[self.current_vertex]}")
        info_text.append("")
        
        # Pilha atual
        if self.stack:
            stack_str = " ← ".join(reversed(self.stack))  # Mostra do topo para baixo
            info_text.append("📚 PILHA (topo → base):")
            info_text.append(f"   {stack_str}")
        else:
            info_text.append("📚 PILHA: Vazia")
        info_text.append("")
        
        # Visitados
        if self.visited:
            visited_str = ", ".join(sorted(self.visited))
            info_text.append(f"✅ VISITADOS: {visited_str}")
        else:
            info_text.append("✅ VISITADOS: Nenhum")
        info_text.append("")
        
        # Caminho
        if self.path:
            path_str = " → ".join(self.path)
            info_text.append("📋 ORDEM DE VISITA:")
            info_text.append(f"   {path_str}")
        
        # Tempos
        if self.discovery_time:
            info_text.append("")
            info_text.append("⏰ TEMPOS (desc/fin):")
            for vertex in sorted(self.discovery_time.keys()):
                discovery = self.discovery_time[vertex]
                finish = self.finish_time.get(vertex, '-')
                info_text.append(f"   {vertex}: {discovery}/{finish}")
        
        text_content = "\n".join(info_text)
        self.ax2.text(0.05, 0.95, text_content, transform=self.ax2.transAxes, fontsize=10,
                     verticalalignment='top', fontfamily='monospace',
                     bbox=dict(boxstyle='round,pad=0.5', facecolor='lightyellow', alpha=0.8))
        
        # SUBPLOT 3: Árvore DFS
        self.draw_dfs_tree()
        
        plt.tight_layout()
        plt.draw()
        plt.pause(0.1)  # Pequena pausa para renderização
    
    def draw_dfs_tree(self):
        """Desenha a árvore DFS no subplot 3"""
        self.ax3.clear()
        self.ax3.set_title("ÁRVORE DFS", fontsize=12, fontweight='bold', color='green')
        self.ax3.axis('off')
        
        if not self.tree_edges:
            # Se não há árvore ainda, mostra mensagem
            self.ax3.text(0.5, 0.5, "Árvore será\nconstruída\nconforme DFS\nprogredir", 
                         transform=self.ax3.transAxes, fontsize=12, ha='center', va='center',
                         bbox=dict(boxstyle='round,pad=0.5', facecolor='lightgreen', alpha=0.5))
            return
        
        # Cria um grafo da árvore DFS apenas com nós visitados
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
        
        # Usa layout de árvore hierárquico
        try:
            # Tenta criar um layout hierárquico
            root = self.path[0] if self.path else list(self.visited)[0]
            tree_pos = nx.nx_agraph.graphviz_layout(tree_graph, prog='dot', root=root)
        except:
            # Fallback para spring layout
            tree_pos = nx.spring_layout(tree_graph, k=2, iterations=50)
        
        # Desenha as arestas da árvore
        if len(tree_graph.edges()) > 0:
            nx.draw_networkx_edges(tree_graph, tree_pos, edge_color='darkgreen', 
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
        nx.draw_networkx_nodes(tree_graph, tree_pos, node_color=tree_node_colors, 
                              node_size=800, alpha=0.9, ax=self.ax3)
        
        # Adiciona labels com tempos
        tree_labels = {}
        for vertex in tree_graph.nodes():
            if vertex in self.discovery_time and vertex in self.finish_time:
                tree_labels[vertex] = f"{vertex}\n{self.discovery_time[vertex]}/{self.finish_time[vertex]}"
            elif vertex in self.discovery_time:
                tree_labels[vertex] = f"{vertex}\n{self.discovery_time[vertex]}/-"
            else:
                tree_labels[vertex] = vertex
        
        nx.draw_networkx_labels(tree_graph, tree_pos, tree_labels, font_size=9, 
                               font_weight='bold', ax=self.ax3)
        
        # Adiciona informações sobre a árvore
        visited_count = len(self.visited)
        tree_info = f"Nós visitados: {visited_count}\nArestas da árvore: {len(self.tree_edges)}"
        self.ax3.text(0.02, 0.98, tree_info, transform=self.ax3.transAxes, fontsize=10,
                     verticalalignment='top',
                     bbox=dict(boxstyle='round,pad=0.3', facecolor='lightgreen', alpha=0.7))
    
    def create_final_visualization(self, path):
        """Cria a visualização final do DFS com árvore completa"""
        # Limpa o conteúdo anterior
        self.ax1.clear()
        self.ax2.clear()
        self.ax3.clear()
        
        # SUBPLOT 1: Grafo original com árvore DFS
        G = nx.Graph()
        for vertex, neighbors in self.graph.items():
            for neighbor in neighbors:
                G.add_edge(vertex, neighbor)
        
        # Desenha todas as arestas em cinza claro
        nx.draw_networkx_edges(G, self.pos, edge_color='lightgray', width=1, alpha=0.3, ax=self.ax1)
        
        # Desenha a árvore DFS em verde
        if self.tree_edges:
            tree_graph = nx.Graph()
            tree_graph.add_edges_from(self.tree_edges)
            nx.draw_networkx_edges(tree_graph, self.pos, edge_color='green', 
                                 width=4, ax=self.ax1)
        
        # Desenha os vértices - todos visitados em verde
        nx.draw_networkx_nodes(G, self.pos, node_color='lightgreen', 
                              node_size=1500, alpha=0.9, ax=self.ax1)
        
        # Labels dos vértices com tempos
        final_labels = {}
        for vertex in self.vertices:
            if vertex in self.discovery_time and vertex in self.finish_time:
                final_labels[vertex] = f"{vertex}\n{self.discovery_time[vertex]}/{self.finish_time[vertex]}"
            else:
                final_labels[vertex] = vertex
        
        nx.draw_networkx_labels(G, self.pos, final_labels, font_size=12, font_weight='bold', ax=self.ax1)
        
        self.ax1.set_title(f"GRAFO COM ÁRVORE DFS\nOrdem: {' → '.join(path)}", 
                          fontsize=14, fontweight='bold', color='darkgreen')
        self.ax1.axis('off')
        
        # SUBPLOT 2: Resumo do algoritmo
        self.ax2.axis('off')
        
        summary_text = []
        summary_text.append("📊 RESUMO DO ALGORITMO DFS")
        summary_text.append("=" * 30)
        summary_text.append("")
        summary_text.append("🎯 OBJETIVO:")
        summary_text.append("   Explorar o grafo em profundidade,")
        summary_text.append("   indo o mais fundo possível")
        summary_text.append("   antes de fazer backtracking")
        summary_text.append("")
        summary_text.append("🌳 ÁRVORE DFS:")
        summary_text.append(f"   • {len(self.tree_edges)} arestas da árvore")
        summary_text.append(f"   • {len(path)} vértices visitados")
        summary_text.append("")
        summary_text.append("⏰ TEMPOS DE DESCOBERTA/FINALIZAÇÃO:")
        for vertex in path:
            discovery = self.discovery_time.get(vertex, 'N/A')
            finish = self.finish_time.get(vertex, 'N/A')
            summary_text.append(f"   {vertex}: {discovery}/{finish}")
        summary_text.append("")
        summary_text.append("🏆 CARACTERÍSTICAS:")
        summary_text.append("   • Usa pilha (LIFO)")
        summary_text.append("   • Explora em profundidade")
        summary_text.append("   • Faz backtracking")
        summary_text.append("   • Detecta ciclos")
        summary_text.append("")
        summary_text.append("💡 APLICAÇÕES:")
        summary_text.append("   • Detecção de ciclos")
        summary_text.append("   • Ordenação topológica")
        summary_text.append("   • Componentes conexas")
        summary_text.append("   • Busca de caminhos")
        
        text_content = "\n".join(summary_text)
        self.ax2.text(0.05, 0.95, text_content, transform=self.ax2.transAxes, fontsize=9,
                     verticalalignment='top', fontfamily='monospace',
                     bbox=dict(boxstyle='round,pad=0.5', facecolor='lightblue', alpha=0.8))
        
        # SUBPLOT 3: Árvore DFS final
        self.ax3.clear()
        self.ax3.set_title("ÁRVORE DFS COMPLETA", fontsize=14, fontweight='bold', color='darkgreen')
        self.ax3.axis('off')
        
        # Desenha a árvore completa
        if self.tree_edges:
            tree_graph = nx.Graph()
            tree_graph.add_edges_from(self.tree_edges)
            
            # Adiciona todos os vértices visitados
            for vertex in self.visited:
                tree_graph.add_node(vertex)
            
            # Layout hierárquico
            try:
                root = path[0] if path else 'A'
                tree_pos = nx.nx_agraph.graphviz_layout(tree_graph, prog='dot', root=root)
            except:
                tree_pos = nx.spring_layout(tree_graph, k=2, iterations=50)
            
            # Desenha arestas da árvore
            nx.draw_networkx_edges(tree_graph, tree_pos, edge_color='darkgreen', 
                                 width=3, ax=self.ax3)
            
            # Desenha nós da árvore
            nx.draw_networkx_nodes(tree_graph, tree_pos, node_color='lightgreen', 
                                  node_size=1000, alpha=0.9, ax=self.ax3)
            
            # Labels com tempos
            tree_labels = {}
            for vertex in tree_graph.nodes():
                discovery = self.discovery_time.get(vertex, 'N/A')
                finish = self.finish_time.get(vertex, 'N/A')
                tree_labels[vertex] = f"{vertex}\n{discovery}/{finish}"
            
            nx.draw_networkx_labels(tree_graph, tree_pos, tree_labels, font_size=10, 
                                   font_weight='bold', ax=self.ax3)
            
            # Adiciona informação sobre a raiz
            root = path[0] if path else 'A'
            self.ax3.text(0.5, 0.02, f"Raiz: {root}\nTempo formato: descoberta/finalização", 
                         transform=self.ax3.transAxes, fontsize=11, ha='center', 
                         fontweight='bold',
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
    print("🚀 Iniciando demonstração do Algoritmo DFS")
    print("⏳ Aguarde... A visualização será exibida passo a passo\n")
    
    dfs_demo = DFSVisualization()
    
    # Executa o algoritmo
    path = dfs_demo.dfs_algorithm(start_vertex='A')
    
    # Visualização final
    print(f"\n📊 Criando visualização final...")
    dfs_demo.create_final_visualization(path)
    
    print(f"\n✨ Demonstração concluída!")
    print(f"📈 O DFS explorou o grafo em profundidade, construindo uma árvore")
    print(f"   e mostrando os tempos de descoberta e finalização de cada vértice.")

if __name__ == "__main__":
    main()