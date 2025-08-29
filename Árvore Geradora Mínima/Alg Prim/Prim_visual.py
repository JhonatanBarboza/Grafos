import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import heapq
import time
from matplotlib.animation import FuncAnimation

class PrimVisualization:
    def __init__(self):
        # Definindo um grafo de exemplo simples
        self.graph = {
            'A': [('B', 4), ('C', 2)],
            'B': [('A', 4), ('C', 1), ('D', 5)],
            'C': [('A', 2), ('B', 1), ('D', 8), ('E', 10)],
            'D': [('B', 5), ('C', 8), ('E', 2), ('F', 6)],
            'E': [('C', 10), ('D', 2), ('F', 3)],
            'F': [('D', 6), ('E', 3)]
        }
        
        # Lista de todas as arestas para visualização
        self.all_edges = []
        for vertex, neighbors in self.graph.items():
            for neighbor, weight in neighbors:
                if (neighbor, vertex, weight) not in self.all_edges:
                    self.all_edges.append((vertex, neighbor, weight))
        
        self.vertices = list(self.graph.keys())
        
        # Estados para animação
        self.animation_states = []
        self.current_state = 0
        
        # Setup da figura
        self.fig, self.ax = plt.subplots(figsize=(14, 10))
        self.fig.suptitle('Algoritmo de Prim - Árvore Geradora Mínima', 
                         fontsize=16, fontweight='bold')
        
        # Posicionamento fixo dos vértices
        self.pos = {
            'A': (0, 1),
            'B': (1, 2),
            'C': (1, 0),
            'D': (2, 1),
            'E': (3, 0),
            'F': (4, 1)
        }
        
    def run_prim_algorithm(self, start_vertex='A'):
        """Executa o algoritmo de Prim e salva todos os estados"""
        print("🌟 Executando Algoritmo de Prim...")
        print(f"🚀 Iniciando do vértice: {start_vertex}")
        
        # Estado inicial
        self.animation_states.append({
            'title': f'Estado Inicial - Vértice {start_vertex} selecionado',
            'visited': {start_vertex},
            'mst_edges': [],
            'current_edge': None,
            'candidate_edges': [],
            'step_info': f'Iniciando com vértice {start_vertex}',
            'total_weight': 0
        })
        
        # Inicialização do algoritmo
        mst = []
        total_weight = 0
        visited = {start_vertex}
        priority_queue = []
        
        # Adiciona arestas iniciais à fila
        for neighbor, weight in self.graph[start_vertex]:
            heapq.heappush(priority_queue, (weight, start_vertex, neighbor))
        
        # Estado após adicionar candidatos iniciais
        candidate_edges = [(weight, u, v) for weight, u, v in priority_queue]
        self.animation_states.append({
            'title': f'Arestas candidatas de {start_vertex} adicionadas à fila',
            'visited': visited.copy(),
            'mst_edges': mst.copy(),
            'current_edge': None,
            'candidate_edges': candidate_edges,
            'step_info': f'Candidatos: {[(u, v, w) for w, u, v in candidate_edges]}',
            'total_weight': total_weight
        })
        
        step = 1
        
        while priority_queue and len(visited) < len(self.vertices):
            # Pega a aresta de menor peso
            weight, u, v = heapq.heappop(priority_queue)
            
            # Se já foi visitado, pula
            if v in visited:
                continue
            
            # Adiciona à MST
            mst.append((u, v, weight))
            total_weight += weight
            visited.add(v)
            
            # Estado destacando a aresta sendo adicionada
            self.animation_states.append({
                'title': f'Passo {step}: Selecionando aresta {u}-{v} (peso: {weight})',
                'visited': visited.copy(),
                'mst_edges': mst.copy(),
                'current_edge': (u, v, weight),
                'candidate_edges': [(w, x, y) for w, x, y in priority_queue if y not in visited],
                'step_info': f'Adicionada: {u}-{v} (peso: {weight}) | Total: {total_weight}',
                'total_weight': total_weight
            })
            
            # Adiciona novas arestas candidatas
            new_candidates = []
            for neighbor, edge_weight in self.graph[v]:
                if neighbor not in visited:
                    heapq.heappush(priority_queue, (edge_weight, v, neighbor))
                    new_candidates.append((edge_weight, v, neighbor))
            
            # Estado após adicionar novos candidatos
            if new_candidates or priority_queue:
                candidate_edges = [(w, x, y) for w, x, y in priority_queue if y not in visited]
                new_info = f'Novos candidatos de {v}: {[(x, y, w) for w, x, y in new_candidates]}'
                
                self.animation_states.append({
                    'title': f'Passo {step}: Vértice {v} adicionado, atualizando candidatos',
                    'visited': visited.copy(),
                    'mst_edges': mst.copy(),
                    'current_edge': None,
                    'candidate_edges': candidate_edges,
                    'step_info': new_info,
                    'total_weight': total_weight
                })
            
            step += 1
        
        # Estado final
        self.animation_states.append({
            'title': f'🎉 Algoritmo Concluído! MST encontrada',
            'visited': visited.copy(),
            'mst_edges': mst.copy(),
            'current_edge': None,
            'candidate_edges': [],
            'step_info': f'MST completa | Peso total: {total_weight} | Arestas: {len(mst)}',
            'total_weight': total_weight
        })
        
        print(f"✅ Algoritmo concluído! {len(self.animation_states)} estados gerados")
        return mst, total_weight
    
    def draw_graph_state(self, state):
        """Desenha um estado específico do grafo"""
        self.ax.clear()
        
        # Cria o grafo
        G = nx.Graph()
        for edge in self.all_edges:
            G.add_edge(edge[0], edge[1], weight=edge[2])
        
        # Desenha todas as arestas em cinza claro
        nx.draw_networkx_edges(G, self.pos, edge_color='lightgray', 
                              width=1.5, alpha=0.3, ax=self.ax)
        
        # Desenha arestas candidatas em laranja pontilhado
        if state['candidate_edges']:
            candidate_edges_draw = []
            for weight, u, v in state['candidate_edges']:
                if u in state['visited']:  # Só mostra se conecta a vértice visitado
                    candidate_edges_draw.append((u, v))
            
            if candidate_edges_draw:
                candidate_graph = nx.Graph()
                candidate_graph.add_edges_from(candidate_edges_draw)
                nx.draw_networkx_edges(candidate_graph, self.pos, edge_color='orange', 
                                     width=3, alpha=0.8, style='dashed', ax=self.ax)
        
        # Desenha arestas da MST em verde
        if state['mst_edges']:
            mst_graph = nx.Graph()
            for edge in state['mst_edges']:
                mst_graph.add_edge(edge[0], edge[1])
            
            nx.draw_networkx_edges(mst_graph, self.pos, edge_color='green', 
                                  width=5, alpha=0.9, ax=self.ax)
        
        # Destaca a aresta atual em vermelho
        if state['current_edge']:
            current_graph = nx.Graph()
            current_graph.add_edge(state['current_edge'][0], state['current_edge'][1])
            nx.draw_networkx_edges(current_graph, self.pos, edge_color='red', 
                                  width=4, alpha=1.0, ax=self.ax)
        
        # Desenha vértices com cores diferentes
        node_colors = []
        node_sizes = []
        for vertex in self.vertices:
            if vertex in state['visited']:
                node_colors.append('lightgreen')
                node_sizes.append(1500)
            else:
                node_colors.append('lightgray')
                node_sizes.append(1200)
        
        nx.draw_networkx_nodes(G, self.pos, node_color=node_colors, 
                              node_size=node_sizes, alpha=0.9, ax=self.ax)
        
        # Labels dos vértices
        nx.draw_networkx_labels(G, self.pos, font_size=14, font_weight='bold', ax=self.ax)
        
        # Pesos das arestas
        edge_labels = {(edge[0], edge[1]): edge[2] for edge in self.all_edges}
        nx.draw_networkx_edge_labels(G, self.pos, edge_labels, font_size=10, ax=self.ax)
        
        # Título e informações
        self.ax.set_title(f"{state['title']}\n{state['step_info']}", 
                         fontsize=12, pad=20)
        
        # Legenda
        from matplotlib.lines import Line2D
        legend_elements = [
            Line2D([0], [0], marker='o', color='w', markerfacecolor='lightgreen', 
                   markersize=12, label='Vértices na MST'),
            Line2D([0], [0], marker='o', color='w', markerfacecolor='lightgray', 
                   markersize=12, label='Vértices não visitados'),
            Line2D([0], [0], color='green', lw=4, label='Arestas da MST'),
            Line2D([0], [0], color='orange', lw=3, linestyle='--', label='Arestas candidatas'),
            Line2D([0], [0], color='red', lw=3, label='Aresta sendo adicionada')
        ]
        self.ax.legend(handles=legend_elements, loc='upper right')
        
        self.ax.axis('off')
        
        # Informações adicionais na parte inferior
        info_text = f"Estado {self.current_state + 1}/{len(self.animation_states)}"
        if state['total_weight'] > 0:
            info_text += f" | Peso Total: {state['total_weight']}"
        
        self.fig.text(0.5, 0.02, info_text, ha='center', fontsize=10, 
                     bbox=dict(boxstyle='round,pad=0.5', facecolor='lightyellow'))
        
        plt.tight_layout()
        plt.draw()
    
    def animate_algorithm(self, interval=2000):
        """Executa a animação dinâmica"""
        def update_frame(frame):
            if frame < len(self.animation_states):
                self.current_state = frame
                self.draw_graph_state(self.animation_states[frame])
                return []
        
        # Configura a animação
        anim = FuncAnimation(self.fig, update_frame, frames=len(self.animation_states),
                           interval=interval, repeat=True, blit=False)
        
        return anim
    
    def interactive_visualization(self):
        """Visualização interativa com controles de navegação"""
        self.current_state = 0
        
        def on_key(event):
            if event.key == 'right' or event.key == ' ':
                # Próximo estado
                if self.current_state < len(self.animation_states) - 1:
                    self.current_state += 1
                    self.draw_graph_state(self.animation_states[self.current_state])
                    
            elif event.key == 'left':
                # Estado anterior
                if self.current_state > 0:
                    self.current_state -= 1
                    self.draw_graph_state(self.animation_states[self.current_state])
                    
            elif event.key == 'r':
                # Reiniciar
                self.current_state = 0
                self.draw_graph_state(self.animation_states[self.current_state])
                
            elif event.key == 'q':
                # Sair
                plt.close()
        
        # Conecta os eventos de teclado
        self.fig.canvas.mpl_connect('key_press_event', on_key)
        
        # Desenha o estado inicial
        self.draw_graph_state(self.animation_states[0])
        
        # Instruções
        instructions = ("Controles:\n"
                       "→ ou ESPAÇO: Próximo passo\n"
                       "← : Passo anterior\n" 
                       "R: Reiniciar\n"
                       "Q: Sair")
        
        self.fig.text(0.02, 0.98, instructions, transform=self.fig.transFigure,
                     fontsize=9, verticalalignment='top',
                     bbox=dict(boxstyle='round,pad=0.5', facecolor='lightblue', alpha=0.8))
        
        plt.show()

def main():
    """Função principal"""
    print("🚀 Algoritmo de Prim - Visualização Dinâmica")
    print("=" * 45)
    
    # Cria a visualização
    prim_viz = PrimVisualization()
    
    # Executa o algoritmo e gera os estados
    mst, total_weight = prim_viz.run_prim_algorithm('A')
    
    print(f"\n📊 Resultado:")
    print(f"   MST com {len(mst)} arestas")
    print(f"   Peso total: {total_weight}")
    print(f"   Estados gerados: {len(prim_viz.animation_states)}")
    
    print(f"\n🎮 Escolha o modo de visualização:")
    print("1. Automática (animação contínua)")
    print("2. Interativa (controle manual)")
    
    try:
        choice = input("\nDigite 1 ou 2: ").strip()
        
        if choice == '1':
            print("\n🎬 Iniciando animação automática...")
            print("   A animação será repetida continuamente")
            anim = prim_viz.animate_algorithm(interval=2500)  # 2.5 segundos por frame
            plt.show()
            
        else:
            print("\n🎮 Iniciando modo interativo...")
            print("   Use as setas do teclado para navegar!")
            prim_viz.interactive_visualization()
            
    except KeyboardInterrupt:
        print("\n\n👋 Visualização encerrada pelo usuário")
    except Exception as e:
        print(f"\n❌ Erro: {e}")
        print("   Iniciando modo interativo como padrão...")
        prim_viz.interactive_visualization()

if __name__ == "__main__":
    main()