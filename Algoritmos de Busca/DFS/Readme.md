## Busca em Profundidade (DFS)

A **Busca em Profundidade (DFS)** é um algoritmo de exploração de grafos que prioriza a investigação completa de cada ramo antes de retroceder e explorar alternativas. Diferente da busca em largura (BFS), que avança em camadas, a DFS avança "o máximo possível" em um caminho antes de retroceder.

### Características Principais:
- **Estratégia**: Explora recursivamente cada vértice e seus vizinhos não visitados, retrocedendo ("backtracking") quando não há mais opções inexploradas.
- **Estrutura de dados**: Utiliza uma **pilha** (implícita na recursão ou explicitamente implementada) para gerenciar o retrocesso.
- **Aplicações**: Detecção de ciclos, ordenação topológica, componentes conexos, caminhos em labirintos, etc.
- **Funcionamento**: Pode ser implementada com recursão ou pilha iterativa.

### Funcionamento Resumido:
1. Inicia em um vértice \( s \), marcando-o como descoberto (cinza) e registrando o tempo de descoberta \( s.d \).
2. Explora recursivamente cada vizinho não visitado:
   - Para cada vizinho \( v \) não descoberto, define \( v.\pi = u \) (predecessor) e repete o processo a partir de \( v \).
3. Quando todos os vizinhos de \( u \) são explorados, marca \( u \) como finalizado (preto) e registra o tempo de término \( u.f \).
4. Se restarem vértices não visitados, reinicia a busca a partir de um novo vértice (fonte), formando uma **floresta DFS** (múltiplas árvores).

### Propriedades:
- **Carimbos de tempo**: Cada vértice \( u \) tem:
  - \( u.d \): tempo de descoberta (quando é visitado).
  - \( u.f \): tempo de término (quando todos seus vizinhos são processados).
  - Satisfaz \( u.d < u.f \).
- **Cores**:
  - **Branco**: não visitado.
  - **Cinza**: em processamento (já descoberto, mas não finalizado).
  - **Preto**: finalizado (todos os vizinhos explorados).
- **Subgrafo predecessor (\( G_\pi \))**: Forma uma **floresta DFS** (conjunto de árvores), onde as arestas \( (u.\pi, u) \) são arestas de árvore.

### Importância:
- A DFS revela a estrutura hierárquica do grafo e é útil para análise de dependências.
- Os carimbos de tempo permitem inferir relações como descendência em árvores e classificação de arestas (árvore, avanço, retorno, cruzamento).
- Base para algoritmos como ordenação topológica e componentes fortemente conexos (algoritmo de Kosaraju).