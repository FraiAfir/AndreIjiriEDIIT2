#ifndef _PARAMS_H
#define _PARAMS_H

typedef struct parametro Param;

/**
 * Este arquivo define a estrutura de dados e as funções relacionadas ao 
 * processamento dos parâmetros/argumentos passados na linha de comando para o programa.
 * 
 * A estrutura de dados Param é utilizada para armazenar os valores dos parâmetros processados, 
 * e as funções definidas neste arquivo são responsáveis por criar o objeto de Parametro, 
 * processar os argumentos da linha de comando, montar os caminhos completos dos arquivos e diretórios de entrada e saída, 
 * e liberar a memória alocada para o objeto de Parametro.
 * 
 * As funções de processamento dos parâmetros da linha de comando seguem as regras estabelecidas para os parâmetros obrigatórios e opcionais, 
 * e garantem que os caminhos completos sejam formados corretamente para os arquivos e diretórios de entrada e saída.
 */

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta é uma função auxiliar de processarParametros() que trata e monta os caminhos completos dos 
 * arquivos e diretórios de entrada e saída, conforme as regras estabelecidas:
 * - Se o parâmetro -e não for fornecido, assume o diretório atual "./";
 * - Se o parâmetro -o não for fornecido, exibe uma mensagem de erro, pois o diretório de saída é obrigatório;
 * - Para os diretórios de entrada e saída, se o caminho fornecido não terminar com '/', 
 * a função adiciona '/' ao final do caminho para garantir que os caminhos completos sejam formados corretamente;
 * - A função também imprime os valores dos parâmetros processados para verificação;
 * 
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      0 se sucesso. -1 se erro
 * @note        Esta função é chamada dentro de processarParametros() após a leitura dos argumentos da linha de comando, 
 * e é responsável por montar os caminhos completos dos arquivos e diretórios de entrada e saída,
 */
int tratarCaminhosCompletos(Param* param);
/**
 * Esta é uma função auxiliar de processarParametros() que processa os parâmetros/argumentos passados na linha de comando, 
 * conforme as regras estabelecidas:
 * - Parâmetros possíveis: -f (obrigatório), -o (obrigatório), -e (opcional), -q (opcional), -pm (opcional);
 * - A ordem dos parâmetros pode variar;
 * - Exemplo de chamada: programa -f arquivo.geo -o dirSaida -e dirEntrada -q arquivo.qry -pm arquivo.pm
 * 
 * @param param Ponteiro para o objeto criado de Parametro
 * @param argc  Número total de argumentos
 * @param argv  Array dos argumentos
 * @return      0 se sucesso. -1 se erro
 * @note        Esta função é chamada dentro de processarParametros() para ler os argumentos da linha de comando 
 * e preencher os campos do objeto de Parametro,
 */
int processarArgumentosInternos(Param* param, int argc, char* argv[]);
/**
 * Estas são as funções getter para acessar os campos da estrutura de parâmetros, 
 * caso seja necessário acessar os valores dos parâmetros processados em outras partes do programa
 * 
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      Ponteiro para a string do campo solicitado
 */
char* getDirEntradaCompleto(Param* param);
char* getDirSaidaCompleto  (Param* param);
char* getDirEntrada        (Param* param);
char* getNomeGeo           (Param* param);
char* getNomeQry           (Param* param);
char* getNomePM            (Param* param);
void  setDirEntradaCompleto(Param* param, const char* dirEntradaCompleto);
void  setDirEntrada        (Param* param, const char* dirEntrada);
void  setDirSaida          (Param* param, const char* dirSaida);
void  setNomeGeo           (Param* param, const char* nomeGeo);
void  setNomeQry           (Param* param, const char* nomeQry);
void  setNomePM            (Param* param, const char* nomePM);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função cria um objeto de Parametro, 
 * alocando a memória necessária para armazenar os campos dos parâmetros processados a partir da linha de comando.
 * Os campos do objeto de Parametro são inicializados como NULL, 
 * e serão preenchidos posteriormente a partir do processamento dos argumentos da linha de comando.
 * 
 * @return  Ponteiro para o novo objeto criado. NULL em caso de erro
 */
Param* criarParametro();
/**
 * Esta função processa os parâmetros/argumentos passados na linha de comando, 
 * utilizando a função processarArgumentosInternos para ler os argumentos e preencher os campos do objeto de Parametro,
 * e a função tratarCaminhosCompletos para montar os caminhos completos dos arquivos e diretórios de entrada e saída, 
 * conforme as regras estabelecidas.
 * 
 * @param param Ponteiro para o objeto criado de Parametro
 * @param argc  Número total de argumentos
 * @param argv  Array dos argumentos
 * @return      0 se sucesso. -1 se erro
 */
int processarParametros(Param* param, int argc, char* argv[]);
/**
 * Esta função libera a memória alocada para os campos do objeto de Parametro,
 * bem como a memória alocada para o próprio objeto de Parametro.
 * 
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      0 se sucesso. -1 se erro
 */
int freeParametros(Param* param);
/*###############################################################################################*/

#endif