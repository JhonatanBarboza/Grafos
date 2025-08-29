# Árvore Geradora Mínima (AGM) - Abordagem Gulosa

## 📌 Definição do Problema

**Entrada**: Um grafo conexo não dirigido \( G = (V, E) \) com função peso \( \omega : E \rightarrow \mathbb{R} \)

**Objetivo**: Encontrar uma árvore geradora mínima (conjunto de arestas que conecta todos os vértices com peso total mínimo)

---

## 🎯 Estratégia Gulosa - AGM-Genérica

### Algoritmo Base:
```python
AGM-GENÉRICA(G, w)
1   A = Ø
2   while A não formar uma árvore geradora:
3       encontrar uma aresta (u, v) que seja segura para A
4       A = A ∪ {(u, v)}
5   return A
```

### Invariante Fundamental:
**Antes de cada iteração**, \( A \) é subconjunto de alguma árvore geradora mínima.

---

## 🔍 Conceitos Fundamentais

### Corte (S, V-S)
- Partição dos vértices V em dois subconjuntos S e V-S
- **Aresta cruza o corte**: se um extremo está em S e outro em V-S

### Corte que Respeita A
- Nenhuma aresta de A cruza o corte

### Aresta Leve
- Aresta de **peso mínimo** que cruza um corte
- Pode haver múltiplas arestas leves (em caso de empates)

---

## 🛡️ Teorema da Aresta Segura

**Uma aresta (u, v) é segura para A se:**
1. Existe um corte (S, V-S) que **respeita A**
2. (u, v) é uma **aresta leve** que cruza esse corte

### Por que funciona?
- Se A está contido em alguma AGM T
- E (u, v) é aresta leve cruzando corte que respeita A
- Então (u, v) pertence a T (ou a alguma AGM alternativa)

---

## 🧠 Como os Algoritmos Usam Isso

### Algoritmo de Prim:
- Mantém **uma árvore** que cresce gradualmente
- Corte: vértices na árvore × vértices fora
- Aresta segura: aresta de peso mínimo conectando a árvore ao resto

### Algoritmo de Kruskal:
- Mantém **floresta** de árvores que se fundem
- Cortes: componentes conectados atuais
- Aresta segura: aresta de peso mínimo conectando componentes diferentes

---

## ⚡ Complexidade

| Algoritmo | Complexidade | Estrutura de Dados |
|-----------|-------------|-------------------|
| Prim | \( O(E \log V) \) | Heap mínimo |
| Kruskal | \( O(E \log E) \) | Union-Find |

---

## 🎯 Aplicações Práticas

1. **Projeto de redes**: Conexão de pontos com custo mínimo (redes elétricas, telecomunicações)
2. **Clusterização**: Agrupamento de dados com similaridade máxima
3. **Roteamento**: Caminhos de custo mínimo em redes
4. **Approximation algorithms**: Soluções aproximadas para problemas NP-difíceis

---

## 💡 Observações Importantes

1. **Optimalidade**: A estratégia gulosa funciona porque o problema tem propriedade de subestrutura ótima
2. **Unicidade**: Se todos os pesos são distintos, a AGM é única
3. **Generalização**: Funciona para grafos com pesos negativos também
4. **Extensões**: Árvore geradora mínima para grafos não conexos → floresta geradora mínima

A abordagem gulosa para AGM demonstra como uma estratégia simples mas bem fundamentada pode resolver eficientemente um problema de otimização combinatória fundamental.