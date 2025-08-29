# Ordenação Topológica em Grafos Acíclicos Dirigidos (GADs)

## Definição
A **ordenação topológica** de um grafo acíclico dirigido (GAD) \( G = (V, E) \) é uma ordenação linear de seus vértices tal que, para toda aresta dirigida \( (u, v) \), \( u \) aparece antes de \( v \) na ordenação. Essa ordenação representa uma sequência que respeita todas as relações de precedência definidas pelas arestas do grafo.

## Aplicações
- **Planejamento de tarefas**: Sequenciamento de atividades com dependências (exemplo: vestir roupas em ordem correta).
- **Compilação**: Ordenação de módulos com dependências.
- **Gerenciamento de projetos**: Determinação da ordem ideal de execução de tarefas.

## Algoritmo de Ordenação Topológica
```python
1. Executar DFS(G) para calcular tempos de término v.f para cada vértice v
2. À medida que cada vértice é finalizado, inseri-lo no início de uma lista
3. Retornar a lista resultante
```

## Complexidade
- **Tempo**: \( \Theta(V + E) \) (mesma complexidade da DFS)
- **Espaço**: \( O(V) \) para armazenar a lista ordenada

## Propriedades Fundamentais

### Lema
Um grafo dirigido \( G \) é acíclico **se e somente se** uma DFS em \( G \) não produz arestas de retorno.

### Teorema
O algoritmo de ordenação topológica produz uma ordenação válida para qualquer GAD.

## Prova de Corretude
Para qualquer aresta \( (u, v) \) em um GAD:
- Se \( v \) é **branco** durante a exploração de \( u \): \( v \) torna-se descendente de \( u \) ⇒ \( v.f < u.f \)
- Se \( v \) é **preto**: \( v \) já foi finalizado ⇒ \( v.f < u.f \)
- **Nunca** \( v \) é cinza (isso indicaria ciclo)

Portanto, para toda aresta \( (u, v) \), temos \( v.f < u.f \), garantindo que \( u \) apareça antes de \( v \) na ordenação topológica (que usa ordem decrescente de tempos de término).

## Exemplo Prático
No problema de vestir roupas:
- **Arestas**: (meias, sapatos), (calças, sapatos), (calças, cinto), etc.
- **Ordenação topológica**: Fornece uma sequência válida para vestir as peças respeitando todas as precedências.

A ordenação topológica é essencial para problemas de sequenciamento com restrições de precedência em diversas áreas da computação e engenharia.