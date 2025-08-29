/*
 * TAD BFS - Busca em Largura
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define VALOR_INVALIDO -1

// =============================================================================
// DEFINIÇÃO DOS TIPOS DE DADOS
// =============================================================================

// Estrutura para representar uma fila
typedef struct {
    int elementos[MAX_VERTICES];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// Estrutura para um nó da lista de adjacência
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

// Estrutura principal do grafo
typedef struct {
    int numeroVertices;
    No** listaAdjacencia;
    bool* visitado;
    int* distancia;
    int* predecessor;
} Grafo;

// Estrutura para resultado da busca
typedef struct {
    bool encontrado;
    int distancia;
    int* caminho;
    int tamanhoCaminho;
} ResultadoBusca;

// =============================================================================
// OPERAÇÕES DA FILA
// =============================================================================

// Cria uma nova fila vazia
Fila* criarFila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    if (fila == NULL) return NULL;
    
    fila->inicio = 0;
    fila->fim = -1;
    fila->tamanho = 0;
    return fila;
}

// Verifica se a fila está vazia
bool filaVazia(Fila* fila) {
    return (fila->tamanho == 0);
}

// Verifica se a fila está cheia
bool filaCheia(Fila* fila) {
    return (fila->tamanho == MAX_VERTICES);
}

// Insere elemento na fila
bool inserirFila(Fila* fila, int elemento) {
    if (filaCheia(fila)) {
        printf("ERRO: Fila cheia!\n");
        return false;
    }
    
    fila->fim = (fila->fim + 1) % MAX_VERTICES;
    fila->elementos[fila->fim] = elemento;
    fila->tamanho++;
    return true;
}

// Remove elemento da fila
int removerFila(Fila* fila) {
    if (filaVazia(fila)) {
        printf("ERRO: Fila vazia!\n");
        return VALOR_INVALIDO;
    }
    
    int elemento = fila->elementos[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_VERTICES;
    fila->tamanho--;
    return elemento;
}

// Libera memória da fila
void destruirFila(Fila* fila) {
    if (fila != NULL) {
        free(fila);
    }
}

// =============================================================================
// OPERAÇÕES DO GRAFO
// =============================================================================

// Cria um novo grafo
Grafo* criarGrafo(int numeroVertices) {
    if (numeroVertices <= 0 || numeroVertices > MAX_VERTICES) {
        printf("ERRO: Número de vértices inválido!\n");
        return NULL;
    }
    
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) return NULL;
    
    grafo->numeroVertices = numeroVertices;
    
    // Alocar lista de adjacência
    grafo->listaAdjacencia = (No**)malloc(numeroVertices * sizeof(No*));
    grafo->visitado = (bool*)malloc(numeroVertices * sizeof(bool));
    grafo->distancia = (int*)malloc(numeroVertices * sizeof(int));
    grafo->predecessor = (int*)malloc(numeroVertices * sizeof(int));
    
    if (!grafo->listaAdjacencia || !grafo->visitado || 
        !grafo->distancia || !grafo->predecessor) {
        destruirGrafo(grafo);
        return NULL;
    }
    
    // Inicializar listas de adjacência
    for (int i = 0; i < numeroVertices; i++) {
        grafo->listaAdjacencia[i] = NULL;
        grafo->visitado[i] = false;
        grafo->distancia[i] = -1;
        grafo->predecessor[i] = -1;
    }
    
    return grafo;
}

// Cria um novo nó
No* criarNo(int vertice) {
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) return NULL;
    
    novoNo->vertice = vertice;
    novoNo->proximo = NULL;
    return novoNo;
}

// Adiciona uma aresta ao grafo
bool adicionarAresta(Grafo* grafo, int origem, int destino) {
    if (grafo == NULL || origem < 0 || destino < 0 || 
        origem >= grafo->numeroVertices || destino >= grafo->numeroVertices) {
        printf("ERRO: Vértices inválidos!\n");
        return false;
    }
    
    // Adicionar destino na lista de origem
    No* novoNo = criarNo(destino);
    if (novoNo == NULL) return false;
    
    novoNo->proximo = grafo->listaAdjacencia[origem];
    grafo->listaAdjacencia[origem] = novoNo;
    
    return true;
}

// Adiciona aresta bidirecional (grafo não-dirigido)
bool adicionarArestaBidirecional(Grafo* grafo, int vertice1, int vertice2) {
    return adicionarAresta(grafo, vertice1, vertice2) && 
           adicionarAresta(grafo, vertice2, vertice1);
}

// Reinicia os arrays de controle para nova busca
void reiniciarBusca(Grafo* grafo) {
    for (int i = 0; i < grafo->numeroVertices; i++) {
        grafo->visitado[i] = false;
        grafo->distancia[i] = -1;
        grafo->predecessor[i] = -1;
    }
}

// =============================================================================
// ALGORITMO BFS
// =============================================================================

// Executa BFS a partir de um vértice inicial
bool executarBFS(Grafo* grafo, int verticeInicial) {
    if (grafo == NULL || verticeInicial < 0 || 
        verticeInicial >= grafo->numeroVertices) {
        printf("ERRO: Vértice inicial inválido!\n");
        return false;
    }
    
    reiniciarBusca(grafo);
    
    Fila* fila = criarFila();
    if (fila == NULL) return false;
    
    // Inicializar busca
    grafo->visitado[verticeInicial] = true;
    grafo->distancia[verticeInicial] = 0;
    grafo->predecessor[verticeInicial] = -1;
    inserirFila(fila, verticeInicial);
    
    printf("Iniciando BFS a partir do vértice %d:\n", verticeInicial);
    printf("Ordem de visitação: ");
    
    while (!filaVazia(fila)) {
        int verticeAtual = removerFila(fila);
        printf("%d ", verticeAtual);
        
        // Visitar todos os vizinhos não visitados
        No* vizinho = grafo->listaAdjacencia[verticeAtual];
        while (vizinho != NULL) {
            int verticeVizinho = vizinho->vertice;
            
            if (!grafo->visitado[verticeVizinho]) {
                grafo->visitado[verticeVizinho] = true;
                grafo->distancia[verticeVizinho] = grafo->distancia[verticeAtual] + 1;
                grafo->predecessor[verticeVizinho] = verticeAtual;
                inserirFila(fila, verticeVizinho);
            }
            
            vizinho = vizinho->proximo;
        }
    }
    
    printf("\n");
    destruirFila(fila);
    return true;
}

// Busca um vértice específico usando BFS
ResultadoBusca buscarVertice(Grafo* grafo, int origem, int destino) {
    ResultadoBusca resultado = {false, -1, NULL, 0};
    
    if (grafo == NULL || origem < 0 || destino < 0 || 
        origem >= grafo->numeroVertices || destino >= grafo->numeroVertices) {
        printf("ERRO: Vértices inválidos!\n");
        return resultado;
    }
    
    executarBFS(grafo, origem);
    
    if (grafo->visitado[destino]) {
        resultado.encontrado = true;
        resultado.distancia = grafo->distancia[destino];
        
        // Reconstruir caminho
        int* caminho = (int*)malloc(grafo->numeroVertices * sizeof(int));
        int tamanho = 0;
        
        int atual = destino;
        while (atual != -1) {
            caminho[tamanho++] = atual;
            atual = grafo->predecessor[atual];
        }
        
        // Inverter caminho
        resultado.caminho = (int*)malloc(tamanho * sizeof(int));
        resultado.tamanhoCaminho = tamanho;
        
        for (int i = 0; i < tamanho; i++) {
            resultado.caminho[i] = caminho[tamanho - 1 - i];
        }
        
        free(caminho);
    }
    
    return resultado;
}
