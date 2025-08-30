## Busca em Largura (BFS)

A **Busca em Largura (BFS)** é um algoritmo fundamental para explorar grafos, partindo de um vértice de origem \( s \). Ele descobre sistematicamente todos os vértices alcançáveis a partir de \( s \), calculando a distância mínima (em número de arestas) até cada um e construindo uma **árvore de busca em largura** que representa os caminhos mais curtos.

![Image](https://github.com/user-attachments/assets/68964834-7e1e-40dd-9f35-35241057f2b9)

### Características Principais:
- **Funcionamento**: Expande a fronteira de descoberta em "ondas" uniformes, explorando primeiro todos os vértices à mesma distância de \( s \) antes de avançar para distâncias maiores.
- **Estrutura de Dados**: Utiliza uma **fila** (primeiro a entrar, primeiro a sair) para gerenciar a ordem de exploração, garantindo que vértices sejam processados por níveis de distância.
- **Aplicações**: Serve como base para algoritmos clássicos, como o algoritmo de **Prim** (árvore geradora mínima) e **Dijkstra** (caminhos mínimos de origem única).
- **Versatilidade**: Funciona em grafos dirigidos e não dirigidos.

### Funcionamento Resumido:
1. Inicia em \( s \), marcando-o como descoberto (distância 0).
2. Enfileira os vizinhos diretos de \( s \) (distância 1).
3. Processa a fila: para cada vértice desenfileirado, explora seus vizinhos ainda não descobertos, enfileirando-os e atualizando suas distâncias.
4. Repete até esvaziar a fila, tendo descoberto todos os vértices alcançáveis.

A BFS garante que o caminho até qualquer vértice \( v \) na árvore gerada seja o **caminho mais curto** em termos de número de arestas.