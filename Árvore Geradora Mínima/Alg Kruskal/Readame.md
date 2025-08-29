# Algoritmo de Kruskal para Árvore Geradora Mínima (AGM)

## 📋 Visão Geral do Algoritmo

O **algoritmo de Kruskal** é um método guloso para encontrar a árvore geradora mínima (AGM) de um grafo conexo não dirigido com pesos nas arestas. Ele constrói a AGM adicionando iterativamente arestas de menor peso que conectam componentes distintas, evitando a formação de ciclos.

![Image](https://github.com/user-attachments/assets/61076b20-8727-46cd-8e3e-33e9091c7cfd)

# Algoritmo de Kruskal com Visualização

## Pré-requisitos
Instale as dependências necessárias:
```bash
pip install matplotlib networkx numpy
```

## Como Executar
1. Salve o código em `kruskal_visual.py`
2. Execute o programa:
```bash
python kruskal_visual.py
```

## 🧠 Fundamentação Teórica

### Estratégia Gulosa
Kruskal qualifica-se como algoritmo guloso porque em cada etapa seleciona a **aresta de menor peso possível** que conecta duas árvores distintas na floresta em desenvolvimento.

### Base Teórica
- **Corolário 21.2**: Uma aresta leve que conecta duas componentes distintas é **segura** para ser adicionada à AGM
- **Invariante**: O conjunto A mantido sempre é subconjunto de alguma AGM


## ⚙️ Implementação com Estrutura Union-Find

### Pseudocódigo Detalhado:
```python
AGM-KRUSKAL(G, w)
1   A = Ø
2   for cada vértice v ∈ G.V:
3       CRIA-CONJUNTO(v)          # Cada vértice é sua própria árvore
4   edges = lista de todas as arestas em G.E
5   ORDENA(edges) por w em ordem crescente
6   for cada aresta (u, v) em edges:
7       if ENCONTRA-CONJUNTO(u) ≠ ENCONTRA-CONJUNTO(v):
8           A = A ∪ {(u, v)}      # Adiciona aresta segura
9           UNIÃO(u, v)           # Funde as duas árvores
10  return A
```


## 🔍 Funcionamento Passo a Passo

### Fase de Inicialização (Linhas 1-3):
- Inicializa o conjunto A vazio
- Cria |V| conjuntos disjuntos (cada vértice é uma árvore independente)

### Fase de Ordenação (Linhas 4-5):
- Cria lista com todas as arestas (O(E))
- Ordena arestas por peso não decrescente (O(E log E))

### Fase de Construção (Linhas 6-9):
- Itera sobre arestas ordenadas por peso
- Para cada aresta (u, v):
  - Se u e v estão em conjuntos diferentes → aresta não forma ciclo
  - Adiciona (u, v) a A
  - Une os conjuntos de u e v


## ⏱️ Análise de Complexidade

### Operações por Componente:

| Operação | Complexidade | Quantidade |
|----------|-------------|------------|
| CRIA-CONJUNTO | O(1) | |V| |
| ORDENA | O(E log E) | 1 |
| ENCONTRA-CONJUNTO | O(α(V)) | O(E) |
| UNIÃO | O(α(V)) | O(V) |

### Complexidade Total:
- **Tempo de ordenação**: O(E log E)
- **Operações Union-Find**: O((V + E) α(V))
- **Como E ≥ V-1** (grafo conexo): O(E α(V))
- **Total**: O(E log E + E α(V)) = O(E log E)

### Simplificação:
Como |E| < |V|² ⇒ log E = O(log V), temos:
\[
T(n) = O(E \log V)
\]
