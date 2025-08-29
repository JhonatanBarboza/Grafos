/*
 * TAD DFS - Busca em Profundidade
 * Implementação completa em português
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define VALOR_INVALIDO -1

// =============================================================================
// DEFINIÇÃO DOS TIPOS DE DADOS
// =============================================================================

// Estrutura para representar uma pilha
typedef struct {
    int elementos[MAX_VERTICES];
    int topo;
} Pilha;

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
    int* tempoDescoberta;
    int* tempoFinalizacao;
    int* predecessor;
    int tempo;
} Grafo;

// Estrutura para resultado da busca
typedef struct {
    bool encontrado;
    int profundidade;
    int* caminho;
    int tamanhoCaminho;
} ResultadoBusca;

// Enumeração para tipos de arestas
typedef enum {
    ARESTA_ARVORE,      // Aresta da árvore DFS
    ARESTA_RETORNO,     // Aresta de retorno (back edge)
    ARESTA_AVANCO,      // Aresta de avanço (forward edge)
    ARESTA_CRUZAMENTO   // Aresta de cruzamento (cross edge)
} TipoAresta;

// =============================================================================
// OPERAÇÕES DA PILHA
// =============================================================================

// Cria uma nova pilha vazia
Pilha* criarPilha() {
    Pilha* pilha = (Pilha*)malloc(sizeof(Pilha));
    if (pilha == NULL) return NULL;
    
    pilha->topo = -1;
    return pilha;
}

// Verifica se a pilha está vazia
bool pilhaVazia(Pilha* pilha) {
    return (pilha->topo == -1);
}

// Verifica se a pilha está cheia
bool pilhaCheia(Pilha* pilha) {
    return (pilha->topo == MAX_VERTICES - 1);
}

// Insere elemento na pilha (push)
bool empilhar(Pilha* pilha, int elemento) {
    if (pilhaCheia(pilha)) {
        printf("ERRO: Pilha cheia!\n");
        return false;
    }
    
    pilha->elementos[++pilha->topo] = elemento;
    return true;
}

// Remove elemento da pilha (pop)
int desempilhar(Pilha* pilha) {
    if (pilhaVazia(pilha)) {
        printf("ERRO: Pilha vazia!\n");
        return VALOR_INVALIDO;
    }
    
    return pilha->elementos[pilha->topo--];
}

// Consulta o topo da pilha sem remover
int consultarTopo(Pilha* pilha) {
    if (pilhaVazia(pilha)) {
        return VALOR_INVALIDO;
    }
    
    return pilha->elementos[pilha->topo];
}

// Libera memória da pilha
void destruirPilha(Pilha* pilha) {
    if (pilha != NULL) {
        free(pilha);
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
    grafo->tempo = 0;
    
    // Alocar arrays
    grafo->listaAdjacencia = (No**)malloc(numeroVertices * sizeof(No*));
    grafo->visitado = (bool*)malloc(numeroVertices * sizeof(bool));
    grafo->tempoDescoberta = (int*)malloc(numeroVertices * sizeof(int));
    grafo->tempoFinalizacao = (int*)malloc(numeroVertices * sizeof(int));
    grafo->predecessor = (int*)malloc(numeroVertices * sizeof(int));
    
    if (!grafo->listaAdjacencia || !grafo->visitado || !grafo->tempoDescoberta ||
        !grafo->tempoFinalizacao || !grafo->predecessor) {
        destruirGrafo(grafo);
        return NULL;
    }
    
    // Inicializar
    for (int i = 0; i < numeroVertices; i++) {
        grafo->listaAdjacencia[i] = NULL;
        grafo->visitado[i] = false;
        grafo->tempoDescoberta[i] = -1;
        grafo->tempoFinalizacao[i] = -1;
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
    grafo->tempo = 0;
    for (int i = 0; i < grafo->numeroVertices; i++) {
        grafo->visitado[i] = false;
        grafo->tempoDescoberta[i] = -1;
        grafo->tempoFinalizacao[i] = -1;
        grafo->predecessor[i] = -1;
    }
}

// =============================================================================
// ALGORITMO DFS RECURSIVO
// =============================================================================

// Função auxiliar recursiva para DFS
void dfsRecursivoAux(Grafo* grafo, int vertice) {
    // Marcar como visitado e registrar tempo de descoberta
    grafo->visitado[vertice] = true;
    grafo->tempoDescoberta[vertice] = ++(grafo->tempo);
    
    printf("%d ", vertice);
    
    // Visitar todos os vizinhos adjacentes
    No* vizinho = grafo->listaAdjacencia[vertice];
    while (vizinho != NULL) {
        int verticeVizinho = vizinho->vertice;
        
        if (!grafo->visitado[verticeVizinho]) {
            grafo->predecessor[verticeVizinho] = vertice;
            dfsRecursivoAux(grafo, verticeVizinho);
        }
        
        vizinho = vizinho->proximo;
    }
    
    // Registrar tempo de finalização
    grafo->tempoFinalizacao[vertice] = ++(grafo->tempo);
}

// Executa DFS recursivo a partir de um vértice inicial
bool executarDFSRecursivo(Grafo* grafo, int verticeInicial) {
    if (grafo == NULL || verticeInicial < 0 || 
        verticeInicial >= grafo->numeroVertices) {
        printf("ERRO: Vértice inicial inválido!\n");
        return false;
    }
    
    reiniciarBusca(grafo);
    
    printf("Iniciando DFS Recursivo a partir do vértice %d:\n", verticeInicial);
    printf("Ordem de visitação: ");
    
    dfsRecursivoAux(grafo, verticeInicial);
    
    printf("\n");
    return true;
}

// =============================================================================
// ALGORITMO DFS ITERATIVO
// =============================================================================

// Executa DFS iterativo usando pilha
bool executarDFSIterativo(Grafo* grafo, int verticeInicial) {
    if (grafo == NULL || verticeInicial < 0 || 
        verticeInicial >= grafo->numeroVertices) {
        printf("ERRO: Vértice inicial inválido!\n");
        return false;
    }
    
    reiniciarBusca(grafo);
    
    Pilha* pilha = criarPilha();
    if (pilha == NULL) return false;
    
    printf("Iniciando DFS Iterativo a partir do vértice %d:\n", verticeInicial);
    printf("Ordem de visitação: ");
    
    empilhar(pilha, verticeInicial);
    
    while (!pilhaVazia(pilha)) {
        int verticeAtual = desempilhar(pilha);
        
        if (!grafo->visitado[verticeAtual]) {
            grafo->visitado[verticeAtual] = true;
            grafo->tempoDescoberta[verticeAtual] = ++(grafo->tempo);
            printf("%d ", verticeAtual);
            
            // Adicionar vizinhos não visitados à pilha
            // (em ordem reversa para manter consistência com DFS recursivo)
            No* vizinhos[MAX_VERTICES];
            int numVizinhos = 0;
            
            No* atual = grafo->listaAdjacencia[verticeAtual];
            while (atual != NULL) {
                vizinhos[numVizinhos++] = atual;
                atual = atual->proximo;
            }
            
            // Empilhar em ordem reversa
            for (int i = numVizinhos - 1; i >= 0; i--) {
                int verticeVizinho = vizinhos[i]->vertice;
                if (!grafo->visitado[verticeVizinho]) {
                    empilhar(pilha, verticeVizinho);
                    if (grafo->predecessor[verticeVizinho] == -1) {
                        grafo->predecessor[verticeVizinho] = verticeAtual;
                    }
                }
            }
        }
    }
    
    printf("\n");
    destruirPilha(pilha);
    return true;
}

// =============================================================================
// DFS COMPLETO (TODOS OS COMPONENTES)
// =============================================================================

// Executa DFS em todo o grafo (todos os componentes conexos)
void executarDFSCompleto(Grafo* grafo) {
    if (grafo == NULL) {
        printf("ERRO: Grafo inválido!\n");
        return;
    }
    
    reiniciarBusca(grafo);
    
    printf("=== DFS COMPLETO - TODOS OS COMPONENTES ===\n");
    int numComponentes = 0;
    
    for (int i = 0; i < grafo->numeroVertices; i++) {
        if (!grafo->visitado[i]) {
            numComponentes++;
            printf("Componente %d: ", numComponentes);
            dfsRecursivoAux(grafo, i);
            printf("\n");
        }
    }
    
    printf("Total de componentes conexos: %d\n\n", numComponentes);
}

// =============================================================================
// BUSCA DE CAMINHO ESPECÍFICO
// =============================================================================

// Função auxiliar para buscar caminho específico
bool buscarCaminhoAux(Grafo* grafo, int atual, int destino, int* caminho, int* tamanho) {
    grafo->visitado[atual] = true;
    caminho[(*tamanho)++] = atual;
    
    if (atual == destino) {
        return true;
    }
    
    No* vizinho = grafo->listaAdjacencia[atual];
    while (vizinho != NULL) {
        int verticeVizinho = vizinho->vertice;
        
        if (!grafo->visitado[verticeVizinho]) {
            if (buscarCaminhoAux(grafo, verticeVizinho, destino, caminho, tamanho)) {
                return true;
            }
        }
        
        vizinho = vizinho->proximo;
    }
    
    // Backtrack - remover vértice atual do caminho
    (*tamanho)--;
    return false;
}

// Busca um caminho específico usando DFS
ResultadoBusca buscarCaminho(Grafo* grafo, int origem, int destino) {
    ResultadoBusca resultado = {false, -1, NULL, 0};
    
    if (grafo == NULL || origem < 0 || destino < 0 || 
        origem >= grafo->numeroVertices || destino >= grafo->numeroVertices) {
        printf("ERRO: Vértices inválidos!\n");
        return resultado;
    }
    
    reiniciarBusca(grafo);
    
    int* caminhoTemp = (int*)malloc(grafo->numeroVertices * sizeof(int));
    int tamanho = 0;
    
    if (buscarCaminhoAux(grafo, origem, destino, caminhoTemp, &tamanho)) {
        resultado.encontrado = true;
        resultado.profundidade = tamanho - 1;
        resultado.tamanhoCaminho = tamanho;
        resultado.caminho = (int*)malloc(tamanho * sizeof(int));
        
        for (int i = 0; i < tamanho; i++) {
            resultado.caminho[i] = caminhoTemp[i];
        }
    }
    
    free(caminhoTemp);
    return resultado;
}

// =============================================================================
// DETECÇÃO DE CICLOS
// =============================================================================

// Função auxiliar para detectar ciclos em grafo não-dirigido
bool detectarCicloNaoDirigidoAux(Grafo* grafo, int vertice, int pai) {
    grafo->visitado[vertice] = true;
    
    No* vizinho = grafo->listaAdjacencia[vertice];
    while (vizinho != NULL) {
        int verticeVizinho = vizinho->vertice;
        
        if (!grafo->visitado[verticeVizinho]) {
            if (detectarCicloNaoDirigidoAux(grafo, verticeVizinho, vertice)) {
                return true;
            }
        } else if (verticeVizinho != pai) {
            return true; // Ciclo encontrado
        }
        
        vizinho = vizinho->proximo;
    }
    
    return false;
}

// Detecta ciclos em grafo não-dirigido
bool temCiclo(Grafo* grafo) {
    if (grafo == NULL) return false;
    
    reiniciarBusca(grafo);
    
    for (int i = 0; i < grafo->numeroVertices; i++) {
        if (!grafo->visitado[i]) {
            if (detectarCicloNaoDirigidoAux(grafo, i, -1)) {
                return true;
            }
        }
    }
    
    return false;
}
