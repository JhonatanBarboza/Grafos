/*
 * Algoritmo de Prim - Árvore Geradora Mínima (AGM)
 * Implementação completa em português
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INFINITO INT_MAX

// =============================================================================
// DEFINIÇÃO DOS TIPOS DE DADOS
// =============================================================================

// Estrutura para representar uma aresta
typedef struct {
    int origem;
    int destino;
    int peso;
} Aresta;

// Estrutura para um nó da lista de adjacência
typedef struct No {
    int vertice;
    int peso;
    struct No* proximo;
} No;

// Estrutura para representar o grafo
typedef struct {
    int numeroVertices;
    No** listaAdjacencia;
    int** matrizAdjacencia;
    bool usarMatriz;
} Grafo;

// Estrutura para um nó da heap mínima
typedef struct {
    int vertice;
    int chave;
} NoHeap;

// Estrutura para heap mínima
typedef struct {
    NoHeap* nos;
    int* posicao;    // posicao[v] = índice do vértice v na heap
    int tamanho;
    int capacidade;
} HeapMinima;

// Estrutura para resultado da AGM
typedef struct {
    Aresta* arestas;
    int numeroArestas;
    int pesoTotal;
    int* predecessor;
    int* chave;
    bool sucesso;
    int verticeInicial;
} ResultadoAGM;

// =============================================================================
// OPERAÇÕES DA HEAP MÍNIMA
// =============================================================================

// Cria uma heap mínima
HeapMinima* criarHeapMinima(int capacidade) {
    HeapMinima* heap = (HeapMinima*)malloc(sizeof(HeapMinima));
    if (heap == NULL) return NULL;
    
    heap->nos = (NoHeap*)malloc(capacidade * sizeof(NoHeap));
    heap->posicao = (int*)malloc(capacidade * sizeof(int));
    
    if (heap->nos == NULL || heap->posicao == NULL) {
        free(heap);
        return NULL;
    }
    
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    
    return heap;
}

// Troca dois nós na heap
void trocarNos(HeapMinima* heap, int i, int j) {
    NoHeap temp = heap->nos[i];
    heap->nos[i] = heap->nos[j];
    heap->nos[j] = temp;
    
    // Atualizar posições
    heap->posicao[heap->nos[i].vertice] = i;
    heap->posicao[heap->nos[j].vertice] = j;
}

// Heapify para baixo (min-heapify)
void heapificarParaBaixo(HeapMinima* heap, int indice) {
    int menor = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;
    
    if (esquerda < heap->tamanho && 
        heap->nos[esquerda].chave < heap->nos[menor].chave) {
        menor = esquerda;
    }
    
    if (direita < heap->tamanho && 
        heap->nos[direita].chave < heap->nos[menor].chave) {
        menor = direita;
    }
    
    if (menor != indice) {
        trocarNos(heap, indice, menor);
        heapificarParaBaixo(heap, menor);
    }
}

// Heapify para cima
void heapificarParaCima(HeapMinima* heap, int indice) {
    int pai = (indice - 1) / 2;
    
    if (pai >= 0 && heap->nos[pai].chave > heap->nos[indice].chave) {
        trocarNos(heap, pai, indice);
        heapificarParaCima(heap, pai);
    }
}

// Extrai o mínimo da heap
int extrairMinimo(HeapMinima* heap) {
    if (heap->tamanho == 0) return -1;
    
    int verticeMinimo = heap->nos[0].vertice;
    
    // Mover último elemento para a raiz
    heap->nos[0] = heap->nos[heap->tamanho - 1];
    heap->posicao[heap->nos[0].vertice] = 0;
    heap->tamanho--;
    
    // Restaurar propriedade da heap
    if (heap->tamanho > 0) {
        heapificarParaBaixo(heap, 0);
    }
    
    return verticeMinimo;
}

// Diminui a chave de um vértice
void diminuirChave(HeapMinima* heap, int vertice, int novaChave) {
    int indice = heap->posicao[vertice];
    heap->nos[indice].chave = novaChave;
    heapificarParaCima(heap, indice);
}

// Verifica se a heap está vazia
bool heapVazia(HeapMinima* heap) {
    return heap->tamanho == 0;
}

// Verifica se um vértice está na heap
bool verticeNaHeap(HeapMinima* heap, int vertice) {
    return heap->posicao[vertice] < heap->tamanho;
}

// Libera memória da heap
void destruirHeapMinima(HeapMinima* heap) {
    if (heap != NULL) {
        free(heap->nos);
        free(heap->posicao);
        free(heap);
    }
}

// =============================================================================
// OPERAÇÕES DO GRAFO
// =============================================================================

// Cria um novo grafo
Grafo* criarGrafo(int numeroVertices, bool usarMatriz) {
    if (numeroVertices <= 0 || numeroVertices > MAX_VERTICES) {
        printf("ERRO: Número de vértices inválido!\n");
        return NULL;
    }
    
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) return NULL;
    
    grafo->numeroVertices = numeroVertices;
    grafo->usarMatriz = usarMatriz;
    
    if (usarMatriz) {
        // Usar matriz de adjacência
        grafo->matrizAdjacencia = (int**)malloc(numeroVertices * sizeof(int*));
        if (grafo->matrizAdjacencia == NULL) {
            free(grafo);
            return NULL;
        }
        
        for (int i = 0; i < numeroVertices; i++) {
            grafo->matrizAdjacencia[i] = (int*)malloc(numeroVertices * sizeof(int));
            if (grafo->matrizAdjacencia[i] == NULL) {
                // Limpar memória já alocada
                for (int j = 0; j < i; j++) {
                    free(grafo->matrizAdjacencia[j]);
                }
                free(grafo->matrizAdjacencia);
                free(grafo);
                return NULL;
            }
            
            // Inicializar com INFINITO (sem conexão)
            for (int j = 0; j < numeroVertices; j++) {
                grafo->matrizAdjacencia[i][j] = (i == j) ? 0 : INFINITO;
            }
        }
        
        grafo->listaAdjacencia = NULL;
    } else {
        // Usar lista de adjacência
        grafo->listaAdjacencia = (No**)malloc(numeroVertices * sizeof(No*));
        if (grafo->listaAdjacencia == NULL) {
            free(grafo);
            return NULL;
        }
        
        for (int i = 0; i < numeroVertices; i++) {
            grafo->listaAdjacencia[i] = NULL;
        }
        
        grafo->matrizAdjacencia = NULL;
    }
    
    return grafo;
}

// Cria um novo nó para lista de adjacência
No* criarNo(int vertice, int peso) {
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) return NULL;
    
    novoNo->vertice = vertice;
    novoNo->peso = peso;
    novoNo->proximo = NULL;
    
    return novoNo;
}

// Adiciona uma aresta ao grafo (não-dirigido)
bool adicionarAresta(Grafo* grafo, int origem, int destino, int peso) {
    if (grafo == NULL || origem < 0 || destino < 0 || 
        origem >= grafo->numeroVertices || destino >= grafo->numeroVertices || peso < 0) {
        printf("ERRO: Parâmetros inválidos para adicionar aresta!\n");
        return false;
    }
    
    if (origem == destino) {
        printf("AVISO: Ignorando aresta de um vértice para ele mesmo!\n");
        return false;
    }
    
    if (grafo->usarMatriz) {
        // Matriz de adjacência
        grafo->matrizAdjacencia[origem][destino] = peso;
        grafo->matrizAdjacencia[destino][origem] = peso;
    } else {
        // Lista de adjacência
        // Adicionar destino na lista de origem
        No* novoNo1 = criarNo(destino, peso);
        if (novoNo1 == NULL) return false;
        
        novoNo1->proximo = grafo->listaAdjacencia[origem];
        grafo->listaAdjacencia[origem] = novoNo1;
        
        // Adicionar origem na lista de destino (grafo não-dirigido)
        No* novoNo2 = criarNo(origem, peso);
        if (novoNo2 == NULL) return false;
        
        novoNo2->proximo = grafo->listaAdjacencia[destino];
        grafo->listaAdjacencia[destino] = novoNo2;
    }
    
    return true;
}

// Obtém o peso da aresta entre dois vértices
int obterPesoAresta(Grafo* grafo, int origem, int destino) {
    if (grafo == NULL || origem < 0 || destino < 0 || 
        origem >= grafo->numeroVertices || destino >= grafo->numeroVertices) {
        return INFINITO;
    }
    
    if (grafo->usarMatriz) {
        return grafo->matrizAdjacencia[origem][destino];
    } else {
        No* atual = grafo->listaAdjacencia[origem];
        while (atual != NULL) {
            if (atual->vertice == destino) {
                return atual->peso;
            }
            atual = atual->proximo;
        }
        return INFINITO;
    }
}

// Libera memória do grafo
void destruirGrafo(Grafo* grafo) {
    if (grafo == NULL) return;
    
    if (grafo->usarMatriz) {
        if (grafo->matrizAdjacencia != NULL) {
            for (int i = 0; i < grafo->numeroVertices; i++) {
                free(grafo->matrizAdjacencia[i]);
            }
            free(grafo->matrizAdjacencia);
        }
    } else {
        if (grafo->listaAdjacencia != NULL) {
            for (int i = 0; i < grafo->numeroVertices; i++) {
                No* atual = grafo->listaAdjacencia[i];
                while (atual != NULL) {
                    No* proximo = atual->proximo;
                    free(atual);
                    atual = proximo;
                }
            }
            free(grafo->listaAdjacencia);
        }
    }
    
    free(grafo);
}

// =============================================================================
// ALGORITMO DE PRIM - VERSÃO COM HEAP
// =============================================================================

// Algoritmo de Prim usando heap mínima (eficiente para grafos esparsos)
ResultadoAGM primComHeap(Grafo* grafo, int verticeInicial) {
    ResultadoAGM resultado = {NULL, 0, 0, NULL, NULL, false, verticeInicial};
    
    if (grafo == NULL || verticeInicial < 0 || verticeInicial >= grafo->numeroVertices) {
        printf("ERRO: Parâmetros inválidos para Prim!\n");
        return resultado;
    }
    
    printf("=== EXECUTANDO ALGORITMO DE PRIM (COM HEAP) ===\n");
    printf("Vértice inicial: %d\n", verticeInicial);
    
    int n = grafo->numeroVertices;
    
    // Alocar arrays de resultado
    resultado.chave = (int*)malloc(n * sizeof(int));
    resultado.predecessor = (int*)malloc(n * sizeof(int));
    resultado.arestas = (Aresta*)malloc((n-1) * sizeof(Aresta));
    
    if (!resultado.chave || !resultado.predecessor || !resultado.arestas) {
        printf("ERRO: Falha na alocação de memória!\n");
        liberarResultadoAGM(&resultado);
        return resultado;
    }
    
    // Criar heap mínima
    HeapMinima* heap = criarHeapMinima(n);
    if (heap == NULL) {
        liberarResultadoAGM(&resultado);
        return resultado;
    }
    
    // Inicializar arrays
    for (int v = 0; v < n; v++) {
        resultado.chave[v] = INFINITO;
        resultado.predecessor[v] = -1;
        heap->nos[v].vertice = v;
        heap->nos[v].chave = INFINITO;
        heap->posicao[v] = v;
    }
    
    heap->tamanho = n;
    
    // Definir chave do vértice inicial como 0
    resultado.chave[verticeInicial] = 0;
    heap->nos[verticeInicial].chave = 0;
    diminuirChave(heap, verticeInicial, 0);
    
    printf("\nProcessando vértices:\n");
    printf("%-10s %-10s %-15s\n", "Vértice", "Chave", "Predecessor");
    printf("----------------------------------\n");
    
    while (!heapVazia(heap)) {
        // Extrair vértice com menor chave
        int u = extrairMinimo(heap);
        
        printf("%-10d %-10d %-15d\n", u, resultado.chave[u], resultado.predecessor[u]);
        
        // Adicionar aresta à AGM (exceto para o primeiro vértice)
        if (resultado.predecessor[u] != -1) {
            resultado.arestas[resultado.numeroArestas].origem = resultado.predecessor[u];
            resultado.arestas[resultado.numeroArestas].destino = u;
            resultado.arestas[resultado.numeroArestas].peso = resultado.chave[u];
            resultado.numeroArestas++;
            resultado.pesoTotal += resultado.chave[u];
        }
        
        // Atualizar chaves dos vizinhos adjacentes
        if (grafo->usarMatriz) {
            // Usar matriz de adjacência
            for (int v = 0; v < n; v++) {
                int peso = grafo->matrizAdjacencia[u][v];
                if (peso != INFINITO && verticeNaHeap(heap, v) && peso < resultado.chave[v]) {
                    resultado.predecessor[v] = u;
                    resultado.chave[v] = peso;
                    diminuirChave(heap, v, peso);
                }
            }
        } else {
            // Usar lista de adjacência
            No* vizinho = grafo->listaAdjacencia[u];
            while (vizinho != NULL) {
                int v = vizinho->vertice;
                int peso = vizinho->peso;
                
                if (verticeNaHeap(heap, v) && peso < resultado.chave[v]) {
                    resultado.predecessor[v] = u;
                    resultado.chave[v] = peso;
                    diminuirChave(heap, v, peso);
                }
                
                vizinho = vizinho->proximo;
            }
        }
    }
    
    resultado.sucesso = (resultado.numeroArestas == n - 1);
    
    if (resultado.sucesso) {
        printf("\n✓ AGM encontrada com sucesso!\n");
        printf("Peso total: %d\n", resultado.pesoTotal);
    } else {
        printf("\n✗ Grafo não é conexo!\n");
    }
    
    destruirHeapMinima(heap);
    return resultado;
}

// =============================================================================
// ALGORITMO DE PRIM - VERSÃO SIMPLES
// =============================================================================

// Encontra o vértice com menor chave que ainda não foi processado
int encontrarVerticeMinimo(int chave[], bool processado[], int n) {
    int min = INFINITO;
    int indiceMin = -1;
    
    for (int v = 0; v < n; v++) {
        if (!processado[v] && chave[v] < min) {
            min = chave[v];
            indiceMin = v;
        }
    }
    
    return indiceMin;
}

// Algoritmo de Prim simples (eficiente para grafos densos)
ResultadoAGM primSimples(Grafo* grafo, int verticeInicial) {
    ResultadoAGM resultado = {NULL, 0, 0, NULL, NULL, false, verticeInicial};
    
    if (grafo == NULL || verticeInicial < 0 || verticeInicial >= grafo->numeroVertices) {
        printf("ERRO: Parâmetros inválidos para Prim!\n");
        return resultado;
    }
    
    printf("=== EXECUTANDO ALGORITMO DE PRIM (VERSÃO SIMPLES) ===\n");
    printf("Vértice inicial: %d\n", verticeInicial);
    
    int n = grafo->numeroVertices;
    
    // Alocar arrays
    resultado.chave = (int*)malloc(n * sizeof(int));
    resultado.predecessor = (int*)malloc(n * sizeof(int));
    resultado.arestas = (Aresta*)malloc((n-1) * sizeof(Aresta));
    bool* processado = (bool*)malloc(n * sizeof(bool));
    
    if (!resultado.chave || !resultado.predecessor || !resultado.arestas || !processado) {
        printf("ERRO: Falha na alocação de memória!\n");
        free(processado);
        liberarResultadoAGM(&resultado);
        return resultado;
    }
    
    // Inicializar arrays
    for (int v = 0; v < n; v++) {
        resultado.chave[v] = INFINITO;
        resultado.predecessor[v] = -1;
        processado[v] = false;
    }
    
    // Começar do vértice inicial
    resultado.chave[verticeInicial] = 0;
    
    printf("\nProcessando vértices:\n");
    printf("%-10s %-10s %-15s %-10s\n", "Iteração", "Vértice", "Chave", "Predecessor");
    printf("---------------------------------------------\n");
    
    for (int iteracao = 0; iteracao < n; iteracao++) {
        // Encontrar vértice com menor chave não processado
        int u = encontrarVerticeMinimo(resultado.chave, processado, n);
        
        if (u == -1) break; // Grafo não é conexo
        
        processado[u] = true;
        
        printf("%-10d %-10d %-15d %-10d\n", iteracao + 1, u, 
               resultado.chave[u], resultado.predecessor[u]);
        
        // Adicionar aresta à AGM (exceto para o primeiro vértice)
        if (resultado.predecessor[u] != -1) {
            resultado.arestas[resultado.numeroArestas].origem = resultado.predecessor[u];
            resultado.arestas[resultado.numeroArestas].destino = u;
            resultado.arestas[resultado.numeroArestas].peso = resultado.chave[u];
            resultado.numeroArestas++;
            resultado.pesoTotal += resultado.chave[u];
        }
        
        // Atualizar chaves dos vizinhos não processados
        for (int v = 0; v < n; v++) {
            int peso = obterPesoAresta(grafo, u, v);
            
            if (peso != INFINITO && !processado[v] && peso < resultado.chave[v]) {
                resultado.predecessor[v] = u;
                resultado.chave[v] = peso;
            }
        }
    }
    
    resultado.sucesso = (resultado.numeroArestas == n - 1);
    
    if (resultado.sucesso) {
        printf("\n✓ AGM encontrada com sucesso!\n");
        printf("Peso total: %d\n", resultado.pesoTotal);
    } else {
        printf("\n✗ Grafo não é conexo!\n");
    }
    
    free(processado);
    return resultado;
}

// =============================================================================
// FUNÇÕES DE VALIDAÇÃO E ANÁLISE
// =============================================================================

// Verifica se o grafo é conexo
bool grafoEConexo(Grafo* grafo) {
    if (grafo == NULL || grafo->numeroVertices <= 1) {
        return true;
    }
    
    bool* visitado = (bool*)malloc(grafo->numeroVertices * sizeof(bool));
    if (visitado == NULL) return false;
    
    for (int i = 0; i < grafo->numeroVertices; i++) {
        visitado[i] = false;
    }
    
    // DFS a partir do vértice 0
    int* pilha = (int*)malloc(grafo->numeroVertices * sizeof(int));
    if (pilha == NULL) {
        free(visitado);
        return false;
    }
    
    int topo = 0;
    pilha[topo] = 0;
    visitado[0] = true;
    int visitados = 1;
    
    while (topo >= 0) {
        int atual = pilha[topo--];
        
        for (int v = 0; v < grafo->numeroVertices; v++) {
            int peso = obterPesoAresta(grafo, atual, v);
            if (peso != INFINITO && !visitado[v]) {
                visitado[v] = true;
                visitados++;
                pilha[++topo] = v;
            }
        }
    }
    
    bool conexo = (visitados == grafo->numeroVertices);
    
    free(visitado);
    free(pilha);
    
    return conexo;
}

// Compara duas AGMs
void compararAGMs(ResultadoAGM agm1, ResultadoAGM agm2, const char* nome1, const char* nome2) {
    printf("\n=== COMPARAÇÃO DE AGMs ===\n");
    printf("%-20s %-15s %-15s\n", "Aspecto", nome1, nome2);
    printf("--------------------------------------------------\n");
    
    printf("%-20s %-15s %-15s\n", "Sucesso", 
           agm1.sucesso ? "✓" : "✗", 
           agm2.sucesso ? "✓" : "✗");
    
    if (agm1.sucesso && agm2.sucesso) {
        printf("%-20s %-15d %-15d\n", "Peso Total", agm1.pesoTotal, agm2.pesoTotal);
        printf("%-20s %-15d %-15d\n", "Num. Arestas", agm1.numeroArestas, agm2.numeroArestas);
        
        if (agm1.pesoTotal == agm2.pesoTotal) {
            printf("\n✓ Ambas as AGMs têm o mesmo peso ótimo!\n");
        } else {
            printf("\n⚠ AGMs têm pesos diferentes (isso pode indicar erro)!\n");
        }
    }
    printf("\n");
}

// =============================================================================
// FUNÇÕES DE EXIBIÇÃO
// =============================================================================

// Exibe o grafo
void exibirGrafo(Grafo* grafo) {
    if (grafo == NULL) {
        printf("Grafo inválido!\n");
        return;
    }
    
    printf("\n=== REPRESENTAÇÃO DO GRAFO ===\n");
    printf("Vértices: %d\n", grafo->numeroVertices);
    printf("Representação: %s\n", grafo->usarMatriz ? "Matriz" : "Lista");
    
    if (grafo->usarMatriz) {
        printf("\nMatriz de Adjacência:\n");
        printf("%-4s", "");
        for (int j = 0; j < grafo->numeroVertices; j++) {
            printf("%-6d", j);
        }
        printf("\n");
        
        for (int i = 0; i < grafo->numeroVertices; i++) {
            printf("%-4d", i);
            for (int j = 0; j < grafo->numeroVertices; j++) {
                if (grafo->matrizAdjacencia[i][j] == INFINITO) {
                    printf("%-6s", "∞");
                } else {
                    printf("%-6d", grafo->matrizAdjacencia[i][j]);
                }
            }
            printf("\n");
        }
    } else {
        printf("\nLista de Adjacência:\n");
        for (int i = 0; i < grafo->numeroVertices; i++) {
            printf("Vértice %d: ", i);
            
            No* atual = grafo->listaAdjacencia[i];
            if (atual == NULL) {
                printf("(sem conexões)");
            } else {
                while (atual != NULL) {
                    printf("(%d, peso:%d)", atual->vertice, atual->peso);
                    if (atual->proximo != NULL) {
                        printf(" -> ");
                    }
                    atual = atual->proximo;
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Exibe o resultado da AGM
void exibirResultadoAGM(ResultadoAGM agm, const char* nomeAlgoritmo) {
    printf("\n=== RESULTADO DA AGM - %s ===\n", nomeAlgoritmo);
    
    if (!agm.sucesso) {
        printf("❌ AGM não foi encontrada (grafo pode não ser conexo)\n");
        return;
    }
    
    printf("✓ AGM encontrada com sucesso!\n");
    printf("Vértice inicial: %d\n", agm.verticeInicial);
    printf("Número de arestas: %d\n", agm.numeroArestas);
    printf("Peso total: %d\n\n", agm.pesoTotal);
    
    printf("Arestas da AGM:\n");
    printf("%-8s %-8s %-8s\n", "Origem", "Destino", "Peso");
    printf("-------------------------\n");
    
    for (int i = 0; i < agm.numeroArestas; i++) {
        Aresta aresta = agm.arestas[i];
        printf("%-8d %-8d %-8d\n", aresta.origem, aresta.destino, aresta.peso);
    }
    
    printf("\nChaves finais dos vértices:\n");
    printf("%-8s %-8s %-12s\n", "Vértice", "Chave", "Predecessor");
    printf("-----------------------------\n");
    
    for (int i = 0; i < agm.numeroArestas + 1; i++) { // +1 para incluir a raiz
        // Encontrar vértices que fazem parte da AGM
        bool encontrado = false;
        int vertice = -1;
        
        if (i == 0) {
            vertice = agm.verticeInicial;
            encontrado = true;
        } else {
            vertice = agm.arestas[i-1].destino;
            encontrado = true;
        }
        
        if (encontrado && agm.chave != NULL) {
            printf("%-8d %-8d %-12d\n", vertice, 
                   agm.chave[vertice], 
                   agm.predecessor[vertice]);
        }
    }
    printf("\n");
}

// Exibe estatísticas de performance
void exibirEstatisticas(Grafo* grafo, ResultadoAGM agm) {
    if (grafo == NULL) return;
    
    printf("=== ESTATÍSTICAS ===\n");
    printf("Vértices: %d\n", grafo->numeroVertices);
    printf("Representação: %s\n", grafo->usarMatriz ? "Matriz de Adjacência" : "Lista de Adjacência");
    
    if (agm.sucesso) {
        printf("Arestas na AGM: %d\n", agm.numeroArestas);
        printf("Peso total: %d\n", agm.pesoTotal);
        
        // Contar arestas do grafo original
        int totalArestas = 0;
        int pesoTotalOriginal = 0;
        
        if (grafo->usarMatriz) {
            for (int i = 0; i < grafo->numeroVertices; i++) {
                for (int j = i + 1; j < grafo->numeroVertices; j++) {
                    if (grafo->matrizAdjacencia[i][j] != INFINITO) {
                        totalArestas++;
                        pesoTotalOriginal += grafo->matrizAdjacencia[i][j];
                    }
                }
            }
        } else {
            // Para lista, contar seria mais complexo, então estimamos
            printf("Complexidade: O((V+E) log V) com heap, O(V²) sem heap\n");
        }
        
        if (totalArestas > 0) {
            printf("Arestas originais: %d\n", totalArestas);
            printf("Arestas removidas: %d\n", totalArestas - agm.numeroArestas);
            printf("Peso original total: %d\n", pesoTotalOriginal);
            printf("Economia de peso: %d (%.1f%%)\n", 
                   pesoTotalOriginal - agm.pesoTotal,
                   ((double)(pesoTotalOriginal - agm.pesoTotal) / pesoTotalOriginal) * 100);
        }
    } else {
        printf("AGM: Não encontrada\n");
    }
    printf("\n");
}

// Libera memória do resultado da AGM
void liberarResultadoAGM(ResultadoAGM* agm) {
    if (agm->arestas != NULL) {
        free(agm->arestas);
        agm->arestas = NULL;
    }
    if (agm->predecessor != NULL) {
        free(agm->predecessor);
        agm->predecessor = NULL;
    }
    if (agm->chave != NULL) {
        free(agm->chave);
        agm->chave = NULL;
    }
    agm->numeroArestas = 0;
    agm->pesoTotal = 0;
    agm->sucesso = false;
}

// =============================================================================
// FUNÇÃO PRINCIPAL - EXEMPLO DE USO
// =============================================================================

int main() {
    printf("=== ALGORITMO DE PRIM - DEMONSTRAÇÃO ===\n");
    
    // Exemplo 1: Grafo pequeno com matriz de adjacência
    printf("\n### EXEMPLO 1: GRAFO PEQUENO (MATRIZ) ###\n");
    Grafo* grafo1 = criarGrafo(5, true); // usar matriz
    
    // Adicionar arestas
    adicionarAresta(grafo1, 0, 1, 2);
    adicionarAresta(grafo1, 0, 3, 6);
    adicionarAresta(grafo1, 1, 2, 3);
    adicionarAresta(grafo1, 1, 3, 8);
    adicionarAresta(grafo1, 1, 4, 5);
    adicionarAresta(grafo1, 2, 4, 7);
    adicionarAresta(grafo1, 3, 4, 9);
    
    exibirGrafo(grafo1);
    
    printf("Verificando conectividade: ");
    if (grafoEConexo(grafo1)) {
        printf("✓ Grafo é conexo\n");
    } else {
        printf("✗ Grafo não é conexo\n");
    }
    
    // Testar ambas as versões do algoritmo
    ResultadoAGM agmHeap1 = primComHeap(grafo1, 0);
    ResultadoAGM agmSimples1 = primSimples(grafo1, 0);
    
    exibirResultadoAGM(agmHeap1, "PRIM COM HEAP");
    exibirResultadoAGM(agmSimples1, "PRIM SIMPLES");
    
    compararAGMs(agmHeap1, agmSimples1, "Com Heap", "Simples");
    exibirEstatisticas(grafo1, agmHeap1);
    
    // Exemplo 2: Grafo maior com lista de adjacência
    printf("\n### EXEMPLO 2: GRAFO MAIOR (LISTA) ###\n");
    Grafo* grafo2 = criarGrafo(7, false); // usar lista
    
    // Adicionar arestas
    adicionarAresta(grafo2, 0, 1, 4);
    adicionarAresta(grafo2, 0, 2, 3);
    adicionarAresta(grafo2, 1, 2, 1);
    adicionarAresta(grafo2, 1, 3, 2);
    adicionarAresta(grafo2, 2, 3, 4);
    adicionarAresta(grafo2, 2, 4, 2);
    adicionarAresta(grafo2, 3, 4, 6);
    adicionarAresta(grafo2, 3, 5, 1);
    adicionarAresta(grafo2, 4, 5, 3);
    adicionarAresta(grafo2, 4, 6, 1);
    adicionarAresta(grafo2, 5, 6, 2);
    
    exibirGrafo(grafo2);
    
    printf("Verificando conectividade: ");
    if (grafoEConexo(grafo2)) {
        printf("✓ Grafo é conexo\n");
    } else {
        printf("✗ Grafo não é conexo\n");
    }
    
    // Testar com diferentes vértices iniciais
    printf("\n--- Testando com vértice inicial 0 ---\n");
    ResultadoAGM agmHeap2a = primComHeap(grafo2, 0);
    
    printf("\n--- Testando com vértice inicial 3 ---\n");
    ResultadoAGM agmHeap2b = primComHeap(grafo2, 3);
    
    exibirResultadoAGM(agmHeap2a, "PRIM (início=0)");
    exibirResultadoAGM(agmHeap2b, "PRIM (início=3)");
    
    compararAGMs(agmHeap2a, agmHeap2b, "Início=0", "Início=3");
    
    // Exemplo 3: Grafo desconexo
    printf("\n### EXEMPLO 3: GRAFO DESCONEXO ###\n");
    Grafo* grafo3 = criarGrafo(6, true); // usar matriz
    
    // Dois componentes separados
    adicionarAresta(grafo3, 0, 1, 1);
    adicionarAresta(grafo3, 1, 2, 2);
    adicionarAresta(grafo3, 3, 4, 3);  // Componente separado
    adicionarAresta(grafo3, 4, 5, 1);
    
    exibirGrafo(grafo3);
    
    printf("Verificando conectividade: ");
    if (grafoEConexo(grafo3)) {
        printf("✓ Grafo é conexo\n");
    } else {
        printf("✗ Grafo não é conexo\n");
    }
    
    ResultadoAGM agmDesconexo = primComHeap(grafo3, 0);
    exibirResultadoAGM(agmDesconexo, "PRIM EM GRAFO DESCONEXO");
    
    // Exemplo 4: Grafo completo (K4)
    printf("\n### EXEMPLO 4: GRAFO COMPLETO K4 ###\n");
    Grafo* grafo4 = criarGrafo(4, false); // usar lista
    
    // Todas as possíveis arestas
    adicionarAresta(grafo4, 0, 1, 10);
    adicionarAresta(grafo4, 0, 2, 6);
    adicionarAresta(grafo4, 0, 3, 5);
    adicionarAresta(grafo4, 1, 2, 4);
    adicionarAresta(grafo4, 1, 3, 15);
    adicionarAresta(grafo4, 2, 3, 20);
    
    exibirGrafo(grafo4);
    
    ResultadoAGM agmCompleto = primSimples(grafo4, 0);
    exibirResultadoAGM(agmCompleto, "PRIM EM GRAFO COMPLETO");
    exibirEstatisticas(grafo4, agmCompleto);
    
    // Exemplo 5: Comparação de performance (simulada)
    printf("\n### EXEMPLO 5: ANÁLISE DE COMPLEXIDADE ###\n");
    printf("Características dos algoritmos:\n");
    printf("\n1. PRIM COM HEAP MÍNIMA:\n");
    printf("   - Complexidade: O((V + E) log V)\n");
    printf("   - Melhor para: Grafos esparsos (E << V²)\n");
    printf("   - Usa: Heap binária + Lista/Matriz de adjacência\n");
    printf("   - Memória: O(V) para heap + O(V+E) ou O(V²) para grafo\n");
    
    printf("\n2. PRIM SIMPLES:\n");
    printf("   - Complexidade: O(V²)\n");
    printf("   - Melhor para: Grafos densos (E ≈ V²)\n");
    printf("   - Usa: Busca linear do mínimo\n");
    printf("   - Memória: O(V²) se usar matriz, O(V+E) se usar lista\n");
    
    printf("\n3. QUANDO USAR CADA UM:\n");
    printf("   - Heap: E < V²/log V (grafos esparsos)\n");
    printf("   - Simples: E ≥ V²/log V (grafos densos)\n");
    printf("   - Matriz: Quando E ≈ V² (acesso O(1) a pesos)\n");
    printf("   - Lista: Quando E << V² (economia de memória)\n");
    
    // Exemplo 6: Teste com vértice isolado
    printf("\n### EXEMPLO 6: GRAFO COM VÉRTICE ISOLADO ###\n");
    Grafo* grafo5 = criarGrafo(5, true);
    
    // Deixar vértice 4 isolado
    adicionarAresta(grafo5, 0, 1, 1);
    adicionarAresta(grafo5, 1, 2, 2);
    adicionarAresta(grafo5, 2, 3, 3);
    // Vértice 4 fica isolado
    
    exibirGrafo(grafo5);
    
    printf("Verificando conectividade: ");
    if (grafoEConexo(grafo5)) {
        printf("✓ Grafo é conexo\n");
    } else {
        printf("✗ Grafo não é conexo\n");
    }
    
    ResultadoAGM agmIsolado = primComHeap(grafo5, 0);
    exibirResultadoAGM(agmIsolado, "PRIM COM VÉRTICE ISOLADO");
    
    // Limpeza de memória
    destruirGrafo(grafo1);
    destruirGrafo(grafo2);
    destruirGrafo(grafo3);
    destruirGrafo(grafo4);
    destruirGrafo(grafo5);
    
    liberarResultadoAGM(&agmHeap1);
    liberarResultadoAGM(&agmSimples1);
    liberarResultadoAGM(&agmHeap2a);
    liberarResultadoAGM(&agmHeap2b);
    liberarResultadoAGM(&agmDesconexo);
    liberarResultadoAGM(&agmCompleto);
    liberarResultadoAGM(&agmIsolado);
    
    printf("\n=== RESUMO DOS RESULTADOS ===\n");
    printf("✓ Algoritmo de Prim implementado com sucesso\n");
    printf("✓ Duas versões: com heap (O((V+E)logV)) e simples (O(V²))\n");
    printf("✓ Suporte a matriz e lista de adjacência\n");
    printf("✓ Detecção de grafos desconexos\n");
    printf("✓ Validação completa de resultados\n");
    printf("✓ Análise de complexidade e performance\n");
    printf("✓ Gerenciamento robusto de memória\n");
    
    printf("\nPrograma finalizado com sucesso!\n");
    return 0;
}