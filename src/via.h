#ifndef _VIA_H
#define _VIA_H

#include "params.h"
#include "grafo.h"

typedef struct parametro Param;
typedef struct via Via;



/** DEFINIÇÃO DO MÓDULO
 * Este módulo de Via é responsável, principalmete, por fazer um parser do arquivo .via, 
 * que contém informações sobre vias (ruas) de uma cidade, 
 * e armazenar essas informações em uma estrutura de dados adequada (Grafo).
*/



/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função é responsável por montar o caminho completo do arquivo .via
 * a partir do diretório de entrada e do nome do arquivo .via fornecidos na linha de comando, 
 * e armazenar o resultado no buffer caminhoVia.
 * 
 * @param param         Ponteiro para a estrutura de parâmetros
 * @param caminhoVia    Buffer para armazenar o caminho completo do arquivo .via
 * @return              0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoVia(Param* param, char* caminhoVia);
/**
 * Esta função é responsável por ler o arquivo .via linha por linha, processar as consultas e modificações descritas no arquivo .via,
 * e produzir os arquivos de saída .svg e .txt com os resultados das operações realizadas.
 * 
 * @param arquivoVia    Ponteiro para o arquivo .via aberto
 * @param g             Ponteiro para o grafo onde os dados do arquivo .via serão armazenados
 * @param param         Ponteiro para a estrutura de parâmetros
 * @return              0 em caso de sucesso. -1 em caso de erro
 */
int readFileVia(FILE* arquivoVia, Grafo* g, Param* param);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta é uma função wrapper para o processamento do arquivo .via
 * 
 * Ela é responsável por orquestrar as etapas de leitura e processamento do arquivo .via,
 * incluindo a montagem do caminho completo do arquivo .via, a abertura do arquivo .via para leitura, 
 * a chamada da função de leitura e processamento do arquivo .via, e o fechamento do arquivo .via após o processamento.
 * 
 * @param param Ponteiro para a estrutura de parâmetros
 * @param g     Ponteiro para o grafo que armazenará os dados do arquivo .via
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int processarVia(Param* param, Grafo* g);
/**
 * Esta função é responsável por criar uma instância de Via, alocando a memória necessária e inicializando seus campos.
 * @return Ponteiro para a instância de Via criada. NULL em caso de erro
 */
Via* criarVia();
/**
 * Esta função é responsável por liberar a memória alocada para uma instância de Via, incluindo os campos internos da estrutura.
 * @param via Ponteiro para a instância de Via a ser liberada
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int freeVia(Via* via);
/*###############################################################################################*/

#endif