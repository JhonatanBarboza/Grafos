/**
 * @file BFS.h
 * @brief Assinatura das funções de implementação do algoritmo de Busca em Largura (BFS).
 */
#ifndef BFS_H
    #define BFS_H

    // inclui as bibliotecas padrão
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    // definição das estruturas de dados
    typedef struct Fila_ Fila;
    typedef struct No_ No;
    typedef struct Grafo_ Grafo;
    typedef struct ResultadoBusca_ ResultadoBusca;

    // =============================================================================
    // Definição das funções
    // =============================================================================

    // Operações da fila
    Fila* criarFila();
    bool filaVazia(Fila* fila);
    bool filaCheia(Fila* fila);
    bool inserirFila(Fila* fila, int elemento);
    int removerFila(Fila* fila);
    void destruirFila(Fila* fila);

    // Operações do grafo
    Grafo* criarGrafo(int numeroVertices);
    No* criarNo(int vertice);
    bool adicionarAresta(Grafo* grafo, int origem, int destino);
    bool adicionarArestaBidirecional(Grafo* grafo, int vertice1, int vertice2);
    void reiniciarBusca(Grafo* grafo);
    bool executarBFS(Grafo* grafo, int verticeInicial);
    ResultadoBusca buscarVertice(Grafo* grafo, int origem, int destino);

#endif