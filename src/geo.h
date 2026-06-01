#ifndef _GEO_H
#define _GEO_H

#include "params.h"
#include "hashBin.h"

typedef struct parametro Param;

/**
 * Este contrato contém as declarações das funções relacionadas ao processamento do arquivo .geo,
 * incluindo a leitura dos dados do arquivo .geo, o armazenamento desses dados em uma estrutura de dados adequada (Tabela Hash e Quadras), 
 * e a geração de um arquivo .svg com a representação gráfica dos dados do arquivo .geo.
 * 
 * As funções auxiliares são responsáveis por tarefas específicas, 
 * como montar o caminho completo do arquivo .geo e ler o arquivo linha por linha para processar os comandos encontrados.
 * 
 * A função principal é responsável por coordenar o processo de leitura e processamento do arquivo .geo,
 * garantindo que os dados sejam armazenados corretamente e que o arquivo .svg seja gerado com a representação gráfica adequada. 
 * 
 * O arquivo .geo é um arquivo de entrada que contém informações sobre a cidade de Bitnópolis, como as quadras, os pontos de interesse, etc. 
 * O processamento desse arquivo é fundamental para a execução do programa, 
 * pois os dados lidos do arquivo .geo serão utilizados em outras partes do programa, 
 * como no processamento do arquivo .pm e na geração de relatórios e visualizações. 
 * 
 * As funções declaradas neste arquivo são implementadas no arquivo geo.c, 
 * onde a lógica de leitura, processamento e armazenamento dos dados do arquivo .geo é detalhada, 
 * garantindo que o programa possa utilizar esses dados de maneira eficiente em outras partes do código. 
 * 
 * A estrutura de dados utilizada para armazenar os dados do arquivo .geo é um Hash File Dinâmico Binário,
 * que permite um acesso eficiente aos dados, facilitando as operações de inserção, 
 * busca e modificação dos dados relacionados às quadras e outros elementos da cidade de Bitnópolis.
 */

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função monta o caminho completo do arquivo .geo a partir do diretório de entrada 
 * e do nome do arquivo .geo fornecidos nos parâmetros da linha de comando, e armazena o caminho completo no buffer fornecido.
 * 
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoGeo Buffer para armazenar o caminho completo do arquivo .geo
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoGeo(Param* param, char caminhoGeo[512]);
/**
 * Esta função lê o arquivo .geo linha por linha, 
 * processa os comandos encontrados e armazena os dados em uma estrutura de dados adequada (Tabela Hash e Quadras).
 * 
 * @param arquivoGeo Ponteiro para o arquivo .geo aberto
 * @param dir        Ponteiro para a tabela hash onde os dados do arquivo .geo serão armazenados
 * @param q          Ponteiro para o objeto que armazenará os dados lidos do arquivo .geo
 * @param param      Ponteiro para a estrutura de parâmetros
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int readFileGeo(FILE* arquivoGeo, HashBin* dir, Quadras* q, Param* param);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta é uma função wrapper para o processamento do arquivo .geo. Ela é responsável por coordenar as etapas de processamento do arquivo .geo,
 * incluindo a montagem do caminho completo do arquivo .geo, a abertura do arquivo para leitura, 
 * a verificação da estrutura de dados para armazenar os dados do arquivo .geo, 
 * a leitura e processamento dos dados do arquivo .geo, e o fechamento do arquivo após o processamento.
 * 
 * @param param Ponteiro para a estrutura de parâmetros
 * @param h     Ponteiro para a tabela hash que armazenará os dados do arquivo .geo
 * @param q     Ponteiro para o objeto que armazenará os dados lidos do arquivo .geo
 * @return 0 em caso de sucesso. -1 em caso de erro
 */
int processarGeo(Param* param, HashBin* h, Quadras* q);
/*###############################################################################################*/

#endif