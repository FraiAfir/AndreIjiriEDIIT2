#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct grafo Grafo;
typedef struct boundingbox BoundingBox;



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
/**
 * Esta é uma função auxiliar ao TAD qry que atualiza a velocidade média das arestas 
 * dentro de uma região retangular definida por (x, y, w, h) para uma nova velocidade v.
 * 
 * @param g     Ponteiro para o grafo
 * @param x     Coordenada x do canto superior esquerdo da região retangular
 * @param y     Coordenada y do canto superior esquerdo da região retangular
 * @param w     Largura da região retangular
 * @param h     Altura da região retangular
 * @param v     Nova velocidade média a ser atribuída às arestas dentro da região
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int grafoAtualizarVelocidadeRegiao(Grafo* g, double x, double y, double w, double h, double v);
/**
 * Esta é a função auxiliar ao TAD qry que realiza uma busca em profundidade (DFS)
 * para identificar os componentes conexos do grafo, considerando apenas as arestas com velocidade média maior ou igual a vl.
 * A função marca os vértices visitados com um identificador de ilha (idIlha) e atualiza o bounding box da ilha com as coordenadas dos vértices visitados.
 * 
 * @note Esta função é chamada recursivamente para explorar todos os vértices conectados ao vértice atual,
 * marcando-os como pertencentes à ilha atual e atualizando o bounding box da ilha.
 * @param g             Ponteiro para o grafo
 * @param indiceVertice Índice do vértice atual a ser visitado na DFS
 * @param idIlhaAtual   Identificador da ilha atual
 * @param vl            Velocidade média limite para considerar uma aresta
 * @param bb            Ponteiro para o bounding box da ilha
 */
void DFSIlhas(Grafo* g, int indiceVertice, int idIlhaAtual, double vl, BoundingBox* bb);
/**
 * Esta é uma função getter para obter as extremidades de um bounding box.
 * 
 * @param bb        Ponteiro para o bounding box
 * @param indice    Índice do bounding box no vetor de bounding boxes
 * @param minX      Ponteiro para armazenar a coordenada x mínima
 * @param minY      Ponteiro para armazenar a coordenada y mínima
 * @param maxX      Ponteiro para armazenar a coordenada x máxima
 * @param maxY      Ponteiro para armazenar a coordenada y máxima
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int getExtremidadesBB(BoundingBox* bb, int indice, double* minX, double* minY, double* maxX, double* maxY);
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
int inserirAresta(Grafo* g, int i, int j, char* ldir, char* lesq, double cmp, double vm, char* nomeRua);
/**
 * Esta função é responsável por identificar os componentes conexos do grafo,
 * considerando apenas as arestas com velocidade média maior ou igual a vl.
 * Assim como popular o vetor de bounding boxes (vetorBB) com as extremidades de cada componente conexo identificado.
 * 
 * @note Esta função utiliza a busca em profundidade DFSIlhas(); para explorar os vértices do grafo e identificar os componentes conexos.
 * @param g       Ponteiro para o grafo
 * @param vl      Velocidade média limite para considerar uma aresta
 * @param vetorBB Ponteiro para o vetor de Bounding Boxes
 * @return        Número de componentes conexos identificados. -1 em caso de erro
 */
int identificarComponentesConexos(Grafo* g, double vl, BoundingBox** vetorBB);
/*###############################################################################################*/

#endif