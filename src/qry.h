#ifndef _QRY_H
#define _QRY_H

typedef struct qry Qry;
typedef struct registrador Registrador;
typedef struct parametro Param;

#include "svg.h"
#include "grafo.h"
#include "params.h"
#include "hashBin.h"

/** DEFINIÇÃO DO MÓDULO
 * Este módulo de Qry é responsável, principalmente, por fazer um parser do arquivo .qry.
 * 
 * O qry é um arquivo de texto que contém uma série de comandos de consulta e modificação dos dados armazenados a partir do arquivo .geo,
 * conforme as instruções do arquivo .qry, e produzir os arquivos de saída .svg e .txt com os resultados das operações realizadas.
 * 
 * Este módulo é responsável por processar o arquivo .qry, 
 * realizando as operações de consulta e modificação nos dados armazenados a partir do arquivo .geo,
 * conforme as instruções do arquivo .qry, e produzir os arquivos de saída .svg e .txt com os resultados das operações realizadas.
 */

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função é responsável por montar o caminho completo do arquivo .qry 
 * a partir do diretório de entrada e do nome do arquivo .qry fornecidos na linha de comando, 
 * e armazenar o resultado no buffer caminhoQry.
 * 
 * @param param         Ponteiro para a estrutura de parâmetros
 * @param caminhoQry    Buffer para armazenar o caminho completo do arquivo .qry
 * @return              0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoQry(Param* param, char* caminhoQry);
/**
 * Esta função é responsável por ler o arquivo .qry linha por linha, processar as consultas e modificações descritas no arquivo .qry,
 * e produzir os arquivos de saída .svg e .txt com os resultados das operações realizadas.
 * 
 * @param arquivoQry    Ponteiro para o arquivo .qry aberto
 * @param hash          Ponteiro para a tabela hash de quadras (HashBin)
 * @param param         Ponteiro para a estrutura de parâmetros
 * @param g             Ponteiro para o grafo
 * @return              0 em caso de sucesso. -1 em caso de erro
 */
int readFileQry(FILE* arquivoQry, HashBin* hash, Param* param, Grafo* g);
/**
 * Esta função é responsável por clonar o arquivo .svg base para um novo arquivo .svg do .qry, 
 * que será utilizado para desenhar os resultados das operações do arquivo .qry.
 * 
 * O arquivo .svg do .qry é criado a partir do clone do arquivo .svg base, 
 * e o conteúdo do arquivo .svg base é copiado para o arquivo .svg do .qry até a tag de fechamento </svg>, 
 * que é ignorada para permitir que o arquivo .svg do .qry receba novos desenhos.
 * 
 * @param caminhoSvgBase O caminho completo do arquivo .svg base a ser clonado
 * @param caminhoSvgQry  O caminho completo do arquivo .svg do .qry a ser criado
 * @return Ponteiro para o arquivo .svg do .qry criado, ou NULL em caso de erro
 */
FILE* clonarSvgBase(char* caminhoSvgBase, char* caminhoSvgQry);
/**
 * Esta função é responsável por calcular as coordenadas (x, y) do endereço especificado pelo cep, face e número,
 * com base nas coordenadas e dimensões da quadra correspondente.
 * 
 * @param q        Ponteiro para a quadra correspondente
 * @param face     Face do endereço (N, S, L, O)
 * @param num      Número do endereço
 * @param x    Ponteiro para armazenar a coordenada x calculada
 * @param y    Ponteiro para armazenar a coordenada y calculada
 * @return         0 em caso de sucesso. -1 em caso de erro
 */
int calcularCoordenadaEndereco(Quadras* q, char face, int num, double* posX, double* posY);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta é uma função wrapper para o processamento do arquivo .qry. 
 * 
 * Ela é responsável por orquestrar as etapas de leitura e processamento do arquivo .qry,
 * incluindo a montagem do caminho completo do arquivo .qry, a abertura do arquivo .qry para leitura, 
 * a chamada da função de leitura e processamento do arquivo .qry, e o fechamento do arquivo .qry após o processamento.
 * 
 * @param param Ponteiro para a estrutura de parâmetros
 * @param hash  Ponteiro para a tabela hash de quadras (HashBin)
 * @param g     Ponteiro para o grafo
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int processarQry(Param* param, HashBin* hash, Grafo* g);
/**
 * Esta função é responsável por criar uma instância de Qry, alocando a memória necessária e inicializando seus campos.
 * @return Ponteiro para a instância de Qry criada. NULL em caso de erro
 */
Qry* criarQry();
/**
 * Esta função é responsável por liberar a memória alocada para uma instância de Qry, incluindo os campos internos da estrutura.
 * @param qry Ponteiro para a instância de Qry
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int freeQry(Qry* qry);
/**
 * COMANDO: @o?
 * Armazena a posição geográfica do endereço cep/face/num no registrador reg.
 * 
 * SVG: linha vertical pontilhada vermelha mostrando a posição do endereço 
 * e colocar o número do registrador na outra extremidade do linha (topo da página)
 *  
 * TXT: reportar a coordenada relativa ao endereço
 * 
 * @param reg       Registrador para armazenar a posição geográfica do endereço cep/face/num
 * @param cep       CEP da quadra onde o endereço está localizado
 * @param face      Face do endereço
 * @param num       Número do endereço
 * @param qrySVG    Ponteiro para o arquivo .svg do .qry para desenhar a linha vertical pontilhada vermelha
 * @param qryTXT    Ponteiro para o arquivo .txt do .qry para reportar a coordenada relativa ao endereço
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int armazenarPosGeografica(HashBin* h, char *reg, char *cep, char *face, char *num, FILE* qrySVG, FILE* qryTXT);
/**
 * COMANDO: mvm
 * Atualiza a velocidade média das arestas dentro da região (x,y,w,h) para v.
 * 
 * @param g     Ponteiro para o grafo
 * @param v     Velocidade média a ser atualizada
 * @param x     Coordenada x do canto superior esquerdo da região
 * @param y     Coordenada y do canto superior esquerdo da região
 * @param w     Largura da região
 * @param h     Altura da região
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int atualizarVelocidade(Grafo* g, char *v, char *x, char *y, char *w, char *h);
/**
 * COMANDO: regs
 * Considera como insuficiente os trechos com velocidade média inferior a vl. Calcula os componentes conexos.
 * 
 * SVG: Calcular os bounding boxes de cada componente conexo e desenhar os respectivos retângulos com cores diferentes e transparência de 50%.
 * TXT: reportar o número de componentes conexos.
 * 
 * 
 * @param g         Ponteiro para o grafo
 * @param vl_str    Velocidade média limite para considerar um trecho como insuficiente
 * @param qrySVG    Ponteiro para o arquivo .svg do .qry para desenhar os retângulos dos componentes conexos
 * @param qryTXT    Ponteiro para o arquivo .txt do .qry para reportar o número de componentes conexos
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int calcularComponentesConexos(Grafo* g, char *vl_str, FILE* qrySVG, FILE* qryTXT);
/**
 * COMANDO: exp
 * Calcula a árvore geradora mínima (seleciona apenas as arestas com velocidade média inferior a vl)
 * aumenta em 50% a velocidade média das arestas selecionadas.
 * 
 * SVG: pintar as arestas selecionadas com linha grossa e vermelha. 
 * 
 * @note Esta é a função motor do algoritmo de Kruskal, que é utilizado para calcular a árvore geradora mínima do grafo.
 * Esta função utiliza alguns procedimentos auxiliares para a implementação do algoritmo de Kruskal, como:
 * - aumentaVMArestas(); para selecionar as arestas com velocidade média inferior a vl e aumentar em 50% a velocidade média das arestas selecionadas.
 * - getTamanhoListaArestas(); para obter o total de arestas selecionadas para desenhar as arestas no arquivo .svg do .qry.
 * - getCoordenadasArestaLista(); para obter as coordenadas dos vértices de origem e destino das arestas selecionadas para desenhar as arestas no arquivo .svg do .qry.
 * - destruirListaArestas(); para liberar a memória alocada para a lista de arestas selecionadas.
 * 
 * @param g         Ponteiro para o grafo
 * @param vl_str    Velocidade média limite para selecionar as arestas para a árvore
 * @param qrySVG    Ponteiro para o arquivo .svg do .qry para pintar as arestas selecionadas com linha grossa e vermelha
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int calcularArvoreGeradoraMinima(Grafo* g, char *vl_str, FILE* qrySVG);
/**
 * COMANDO: p?
 * Determina o melhor trajeto entre a origem (reg1) e o destino (reg2). 
 * Desenhar os percursos (mais curto, mais rápido) com as cores cc e cr, respectivamente.
 * 
 * TXT: descrição textual do percurso. Caso destino inacessível, reportar.
 * SVG: mostrar os percursos pintados e animados. Marcar os extremos do percurso com “placas” com as letras I e F.
 * 
 * QRY: Esta função utiliza um procedimento auxiliar getDirecaoCardeal(); para calcular a direção cardeal entre dois pontos (x1, y1) e (x2, y2), que é utilizado para gerar as instruções textuais do percurso no arquivo .txt do .qry.
 * GRAFO: 
 * - Esta função utiliza o algoritmo de Dijkstra para calcular o melhor trajeto entre os vértices de origem e destino no grafo, considerando dois critérios: o caminho mais curto (menor distância) e o caminho mais rápido (menor tempo).
 * - Utiliza também o procedimento auxiliar DijkstraEncontrarVerticeMaisProximo(); para encontrar os vértices mais próximos das coordenadas de origem e destino no grafo, que são utilizados como pontos de partida e chegada para o cálculo do melhor trajeto.
 * - Assim como os getters getCoordenadasPasso(); getTamanhoCaminho(); getNomeRuaPasso(); para obter as informações de cada passo do percurso calculado pelo algoritmo de Dijkstra, que são utilizadas para desenhar os percursos no arquivo .svg do .qry e gerar as instruções textuais no arquivo .txt do .qry.
 *  
 * @param reg1                  Registrador contendo a região de origem
 * @param reg2                  Registrador contendo a região de destino
 * @param cc                    Cor para o percurso mais curto
 * @param cr                    Cor para o percurso mais rápido
 * @param qrySVG                Ponteiro para o arquivo .svg do .qry para desenhar os percursos e marcar os extremos do percurso
 * @param qryTXT                Ponteiro para o arquivo .txt do .qry para reportar
 * @return                      0 em caso de sucesso. -1 em caso de erro
 */
int calcularMelhorTrajeto(Grafo *g, char *reg1, char *reg2, char *cc, char *cr, FILE* qrySVG, FILE* qryTXT);
/*###############################################################################################*/

#endif