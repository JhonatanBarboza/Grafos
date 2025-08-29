# Árvore Geradora Mínima (AGM) - Abordagem Gulosa Detalhada

## Contexto Teórico

O problema da **árvore geradora mínima** (AGM) é um dos problemas fundamentais da teoria dos grafos e otimização combinatória. Dado um grafo conexo não dirigido \( G = (V, E) \) com função peso \( \omega : E \rightarrow \mathbb{R} \), busca-se encontrar uma árvore que conecte todos os vértices com o menor peso total possível.


## Estratégia Gulosa: AGM-GENÉRICA

### Pseudocódigo Expandido:
```python
AGM-GENÉRICA(G, w)
1   A = Ø
2   while A não formar uma árvore geradora:
3       encontrar uma aresta (u, v) que seja segura para A
4       A = A ∪ {(u, v)}
5   return A
```

### Análise do Invariante de Laço:

**Invariante**: Antes de cada iteração, \( A \) é subconjunto de alguma árvore geradora mínima.

- **Inicialização** (Linha 1): \( A = \emptyset \) trivialmente satisfaz o invariante.
- **Manutenção** (Linhas 2-4): A adição de uma aresta **segura** preserva o invariante.
- **Término** (Linha 5): Quando \( A \) forma uma árvore geradora, ela é mínima.


## Definições Formais

### Corte (S, V-S)
Um **corte** \( (S, V-S) \) é uma partição do conjunto de vértices \( V \) em dois subconjuntos disjuntos \( S \) e \( V-S \).

### Aresta que Cruza um Corte
Uma aresta \( (u, v) \) **cruza** o corte \( (S, V-S) \) se:
\[ u \in S \quad \text{e} \quad v \in V-S \quad \text{ou vice-versa} \]

### Corte que Respeita A
Um corte \( (S, V-S) \) **respeita** um conjunto de arestas \( A \) se nenhuma aresta em \( A \) cruza o corte.

### Aresta Leve
Uma aresta é **leve** que cruza um corte se seu peso é mínimo entre todas as arestas que cruzam esse corte.


## 🛡️ Teorema da Aresta Segura

**Teorema**: Seja \( G = (V, E) \) um grafo conexo não dirigido com função peso \( \omega \), seja \( A \) um subconjunto de arestas contido em alguma AGM de \( G \), seja \( (S, V-S) \) um corte que respeita \( A \), e seja \( (u, v) \) uma aresta leve que cruza \( (S, V-S) \). Então, \( (u, v) \) é **segura** para \( A \).

### Prova (Esboço):
1. Seja \( T \) uma AGM que contém \( A \).
2. Se \( (u, v) \in T \), a prova está completa.
3. Caso contrário, \( T \cup \{(u, v)\} \) contém um ciclo.
4. Existe outra aresta \( (x, y) \) no ciclo que cruza o corte \( (S, V-S) \).
5. Como \( (u, v) \) é leve, \( \omega(u, v) \leq \omega(x, y) \).
6. Substituindo \( (x, y) \) por \( (u, v) \) em \( T \), obtemos outra AGM \( T' \) que contém \( A \cup \{(u, v)\} \).


## Implementações Práticas

### Algoritmo de Prim
- **Estratégia**: Mantém uma única árvore que cresce gradualmente.
- **Corte**: Vértices na árvore × vértices fora da árvore.
- **Aresta segura**: Aresta de peso mínimo conectando a árvore ao resto do grafo.
- **Complexidade**: \( O(E \log V) \) com heap de Fibonacci.

![Image](https://github.com/user-attachments/assets/d6f0af39-d10b-4894-9081-eb5269377e6d)

### Algoritmo de Kruskal
- **Estratégia**: Mantém uma floresta de árvores que se fundem.
- **Cortes**: Componentes conectados atuais.
- **Aresta segura**: Aresta de peso mínimo conectando duas componentes diferentes.
- **Complexidade**: \( O(E \log E) \) com union-find eficiente.

![Image](https://github.com/user-attachments/assets/61076b20-8727-46cd-8e3e-33e9091c7cfd)

## ⚡ Complexidade

| Algoritmo | Complexidade | Estrutura de Dados |
|-----------|-------------|-------------------|
| Prim | \( O(E \log V) \) | Heap mínimo |
| Kruskal | \( O(E \log E) \) | Union-Find |


## ⚠️ Casos Especiais

### Pesos Iguais
- Pode existir múltiplas AGMs
- Arestas leves não são únicas
- Algoritmo ainda encontra uma AGM válida

### Grafos Não Conexos
- O conceito se estende para **floresta geradora mínima**
- Cada componente conexa tem sua própria AGM

## 🔗 Relação com Outros Problemas

### Dualidade
- A AGM está relacionada ao problema do **caixeiro viajante** (TSP)
- Serve como base para algoritmos de aproximação

### Extensões
- **Árvore geradora mínima euclidiana**: Pontos no plano
- **Árvore de Steiner**: Conectar subconjunto de vértices


## 💎 Conclusão

A estratégia gulosa para AGM é notável por:
1. **Simplicidade conceitual**: Baseada em cortes e arestas leves
2. **Eficiência prática**: Algoritmos \( O(E \log V) \) implementáveis
3. **Fundamentação teórica sólida**: Invariante de laço e teorema da aresta segura
4. **Aplicabilidade ampla**: Redes, clusterização, otimização
