#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "params.h"
#include "hashBin.h"

/**
 * Este módulo é responsável por implementar as funções de inicialização e encerramento do programa.
 * 
 * As funções de inicialização e encerramento são responsáveis por alocar e liberar os recursos necessários para a execução do programa, 
 * como os objetos de Parametro, as estruturas de dados para armazenar os dados dos arquivos .geo e .pm, 
 * e os objetos para armazenar os dados lidos desses arquivos.
 */

/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função é responsável por inicializar as diversas instâncias do Projeto, como por exemplo, 
 * o objeto de Parametro, as estruturas de dados para armazenar os dados dos arquivos .geo e .pm, e assim por diante.
 * 
 * @param param     Ponteiro para o objeto criado de Parametro
 * @param htq       Ponteiro para a tabela hash criada para armazenar os dados do arquivo .geo
 * @param q         Ponteiro para o objeto criado para armazenar os dados lidos do arquivo .geo
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int bootProgram(Param** param, HashBin** htq, Quadras** q);
/**
 * Esta função é responsável por liberar a memória alocada para os objetos criados durante a execução do programa, 
 * como por exemplo, o objeto de Parametro, as estruturas de dados para armazenar os dados dos arquivos .geo e .pm, e assim por diante.
 * 
 * @param param Ponteiro para o objeto de Parametro
 * @param htq   Ponteiro para a tabela hash criada para armazenar os dados do arquivo .geo
 * @param q     Ponteiro para o objeto criado para armazenar os dados lidos do arquivo .geo
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int shutProgram(Param** param, HashBin** htq, Quadras** q);

/*###############################################################################################*/

#endif