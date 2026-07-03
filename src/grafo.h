#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct grafo Grafo;



/** DEFINIÇÃO DO MÓDULO
 * Este módulo de Grafo é responsável por representar e manipular grafos, 
 * que são estruturas de dados compostas por vértices (nós) e arestas (conexões entre os nós).
 * 
 * Os grafos podem ser utilizados para modelar uma variedade de problemas, 
 * como redes de transporte, redes sociais, entre outros.
 * 
 * As principais operações suportadas pelo módulo de Grafo incluem 
 * a criação do grafo, adição de vértices e arestas, remoção de vértices e arestas, 
 * busca por caminhos entre vértices, entre outras.
 * 
 * A implementação do módulo de Grafo pode utilizar diferentes representações, como listas de adjacência 
 * ou matrizes de adjacência, dependendo das necessidades específicas do problema a ser resolvido.
 */



/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função é responsável por obter o índice de um vértice no grafo a partir do seu identificador (id).
 * 
 * A função percorre a lista de vértices do grafo e compara o id de cada vértice com o id fornecido como argumento.
 * 
 * Se um vértice com o id correspondente for encontrado, a função retorna o índice desse vértice no grafo. 
 * Caso contrário, a função retorna NULL para indicar que o vértice não foi encontrado.
 * 
 * A função é útil para operações que exigem acesso a um vértice específico no grafo, 
 * como a inserção de arestas ou a busca por caminhos entre vértices.
 * 
 * @param g     Ponteiro para o grafo onde o vértice deve ser buscado
 * @param id    Identificador do vértice a ser buscado
 * @return      Índice do vértice no grafo se encontrado. NULL caso contrário
 */
char* getIndiceVertice(Grafo* g, char* id);
/**
 * Esta função é responsável por obter o número de vértices no grafo.
 * 
 * A função percorre a estrutura de dados do grafo e conta o número de vértices presentes, 
 * retornando esse valor como resultado.
 * 
 * A função é útil para operações que exigem conhecimento do número total de vértices no grafo, 
 * como a alocação de memória para estruturas auxiliares ou a iteração sobre os vértices do grafo.
 * 
 * @param g     Ponteiro para o grafo
 * @return      Número de vértices no grafo
 */
int getNumVertices(Grafo* g);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função é responsável por criar um novo grafo, 
 * alocando a memória necessária para armazenar os vértices e arestas do grafo.
 * 
 * A função deve alocar a memória necessária para armazenar 
 * as informações dos vértices e arestas de acordo com esse limite.
 * 
 * @param maxVertices Número máximo de vértices que o grafo pode conter
 * @return            Ponteiro para o grafo criado. NULL em caso de erro
 */
Grafo* criarGrafo(int maxVertices);
/**
 * Esta função é responsável por liberar a memória alocada para o grafo,
 * incluindo a memória alocada para os vértices e arestas do grafo.
 * 
 * @param g Ponteiro para o grafo a ser liberado
 * @return  0 em caso de sucesso. -1 em caso de erro
 */
int freeGrafo(Grafo* g);
/**
 * Esta função é responsável por inserir um novo vértice no grafo,
 * utilizando um identificador único (id) para o vértice.
 * 
 * @param g     Ponteiro para o grafo
 * @param id    Identificador único do vértice
 * @param x     Coordenada x do vértice
 * @param y     Coordenada y do vértice
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int inserirVertice(Grafo* g, char* id, double x, double y);
/**
 * Esta função é responsável por inserir uma nova aresta no grafo,
 * utilizando os identificadores dos vértices de origem e destino.
 * 
 * @param g         Ponteiro para o grafo
 * @param i         Identificador do vértice de origem da aresta
 * @param j         Identificador do vértice de destino da aresta   
 * @param ldir      Identificador da quadra do lado direito da aresta
 * @param lesq      Identificador da quadra do lado esquerdo da aresta
 * @param cmp       Comprimento da aresta       (peso para o cálculo do caminho mais curto)
 * @param vm        Velocidade média na aresta  (peso para o cálculo do caminho mais rápido)
 * @param nome      Nome da aresta
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int inserirAresta(Grafo* g, char* i, char* j, char* ldir, char* lesq, double cmp, double vm, char* nomeRua);
/*###############################################################################################*/

#endif