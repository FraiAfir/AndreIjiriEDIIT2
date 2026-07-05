#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "via.h"



/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct via{
    // Número de vértices do grafo
    int nv;

    // 1: Comando do arquivo .qry
    char*  comando;
    /** 1.1 - COMANDO: v
     * Cria o vértice id posicionado nas coordenadas [x,y]
     */
    /** 1.2 - COMANDO: e
     * Cria a aresta (i,j) e associa as outras informações à aresta. 
     * Caso a aresta não possua quadras em algum de seus lados, esta ausência é indicada por um hífen (-)
     */

    // 2: Parâmetros associados aos comandos
    /** 2.1: Parâmetros do comando "v" 
     * @param id Identificador do vértice
     * @param x  Coordenada x do vértice
     * @param y  Coordenada y do vértice
     */
    char *id;
    float x, y;
    /** 2.2: Parâmetros do comando "e"
     * @param i     Identificador do vértice de origem da aresta
     * @param j     Identificador do vértice de destino da aresta
     * @param ldir  Identificador da quadra do lado direito da aresta
     * @param lesq  Identificador da quadra do lado esquerdo da aresta
     * @param nome  Nome da via (rua) associada à aresta
     * @param cmp   Comprimento da aresta
     * @param vm    Velocidade média da aresta
     */
    char *i, *j;
    char *ldir, *lesq, *nome;
    float cmp, vm;
}Via;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoVia(Param* param, char* caminhoVia){
    // 1: Montar o caminho completo do arquivo .via
    // a partir do diretório de entrada e do nome do arquivo .via 
    // fornecidos como argumentos na linha de comando, e armazenados na estrutura de parâmetros
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomeVia    = getNomeVia(param);

    // 2: Imprime o nome do arquivo .via original para depuração
    printf("Diretorio de entrada completo: \t\t%s\n", dirEntrada);
    printf("Arquivo .via fornecido: \t\t\t%s\n", nomeVia);
    
    // 3: Concatena o diretório de entrada completo com o nome do arquivo .via
    strcpy(caminhoVia, dirEntrada);
    strcat(caminhoVia, nomeVia);

    // 4: Imprime o caminho completo do arquivo .via para depuração
    printf("Caminho completo do arquivo .via: \t\t%s\n", caminhoVia);

    return 0;
}

int readFileVia(FILE* arquivoVia, Grafo* g, Param* param){
    // 1: Inicializa o buffer para leitura das linhas do arquivo .via
    char linha[256];

    // 2: Parser do arquivo .via linha por linha
    while(fgets(linha, sizeof(linha), arquivoVia) != NULL){
        /** 2.1: Limpeza da linha
         * strcspn(const char *_Str, const char *_Control):
         * - const char *_Str: Ponteiro para a string de entrada que será analisada
         * - const char *_Control: Ponteiro para a string de caracteres de controle que serão buscados na string de entrada
         * 
         * strcspn(linha, "\n"):
         * - linha: Ponteiro para a string de entrada (linha lida do arquivo .via)
         * - "\n": String de controle contendo apenas o caractere de nova linha (ENTER)
         * 
         * strscpn(...) = '\0': Substitui o caractere de nova linha (ENTER) encontrado na linha lida do arquivo .via 
         * por um caractere nulo ('\0'), efetivamente removendo o ENTER do final da linha.
         */
        linha[strcspn(linha, "\n")] = '\0'; // Remove o ENTER do final da linha, se existir
        linha[strcspn(linha, "\r")] = '\0'; // Previne bugs de quebra de linha do Windows
        if(strlen(linha) == 0) continue;    // Ignora linhas em branco

        // 2.2: Extrai apenas o primeiro token da linha, que corresponde ao comando do arquivo .via, para identificar qual comando deve ser processado
        char* bufferLinha = strdup(linha);
        char* comando = NULL;
        comando = strtok(bufferLinha, " ");

        // 2.3: Processa o comando lido do arquivo .via
        if(comando != NULL){
            /** COMANDO: v id x y
             * Cria o vértice id posicionado nas coordenadas [x,y]
             * @param id Identificador do vértice
             * @param x  Coordenada x do vértice
             * @param y  Coordenada y do vértice
             */
            if(strcmp(comando, "v") == 0){
                // Inicializa buffers para os parâmetros
                char id[100] = "";
                char x[100]  = "";
                char y[100]  = "";

                // Extrai os parâmetros
                sscanf(linha, "%*s %s %s %s", id, x, y);

                // Verifica se os parâmetros foram lidos corretamente
                if(id[0] != '\0' && x[0] != '\0' && y[0] != '\0'){
                    if(inserirVertice(g, id, atof(x), atof(y)) != 0){
                        printf("[ERROR]\n");
                        printf("In via.c [readFileVia();]: Failed to insert vertice into the graph\n");
                        return -1;
                    }
                // Caso algum dos parâmetros seja inválido (NULL), imprime uma mensagem de erro e retorna -1
                }else{
                    printf("[ERROR]\n");
                    printf("In via.c [readFileVia();]: Invalid parameters for command 'v' in .via file\n");
                    printf("[id:\t%s]\n[x:\t%s]\n[y:\t%s]\n", id, x, y);
                    return -1;
                }
            }

            /** COMANDO: e i j ldir lesq cmp vm nome
             * Cria a aresta (i,j) e associa as outras informações à aresta. 
             * Caso a aresta não possua quadras em algum de seus lados, esta ausência é indicada por um hífen (-)
             */
            if(strcmp(comando, "e") == 0){
                // Inicializa buffers para os parâmetros
                char  i[100]    = "";
                char  j[100]    = "";
                char  ldir[100] = "";
                char  lesq[100] = "";
                float cmp       = 0.0;
                float vm        = 0.0;
                char  nome[100] = "";

                // Extrai os parâmetros
                sscanf(linha, "%*s %s %s %s %s %f %f %s", i, j, ldir, lesq, &cmp, &vm, nome);
                
                // Verifica se os parâmetros foram lidos corretamente
                if(i[0] != '\0' && j[0] != '\0' && ldir[0] != '\0' && lesq[0] != '\0' && cmp != 0.0 && vm != 0.0 && nome[0] != '\0'){
                    if(inserirAresta(g, i, j, ldir, lesq, cmp, vm, nome) != 0){
                        printf("[ERROR]\n");
                        printf("In via.c [readFileVia();]: Failed to insert arc into the graph\n");
                        return -1;
                    }
                // Caso algum dos parâmetros seja inválido (NULL), imprime uma mensagem de erro e retorna -1
                }else{
                    printf("[ERROR]\n");
                    printf("In via.c [readFileVia();]: Invalid parameters for command 'e' in .via file\n");
                    printf("[i:\t%s]\n[j:\t%s]\n[ldir:\t%s]\n[lesq:\t%s]\n[cmp:\t%f]\n[vm:\t%f]\n[nome:\t%s]\n", 
                        i, j, ldir, lesq, cmp, vm, nome);
                    return -1;
                }
            }
        }

        // 2.4: Se o comando lido do arquivo .qry for NULL, imprime uma mensagem de erro e continua para a próxima linha
        else{
            printf("[ERROR]\n");
            printf("In via.c [readFileVia();]: .via file command is NULL\n");
            return -1;
        }
    }

    // 3: Retorna 0 para indicar sucesso na leitura do arquivo .via
    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarVia(Param* param, Grafo* g){
    // Inicializa o buffer para o caminho completo do arquivo .via
    char caminhoVia[512];

    // 1: Monta o caminho completo do arquivo .via
    if(montarCaminhoVia(param, caminhoVia) != 0){
        printf("[ERROR]\n");
        printf("In via.c [processarVia();]: montarCaminhoVia() returned error\n");
        return -1;
    }

    printf("Iniciando o processamento do arquivo .via\n\n");

    // 2: Abre o arquivo .via para leitura
    FILE* arquivoVia = fopen(caminhoVia, "r");
    if(arquivoVia == NULL){
        printf("[ERROR]\n");
        printf("In via.c [processarVia();]: Failed to open the .via file: %s\n", caminhoVia);
        return -1;
    }

    // 3: Lê e processa os dados do arquivo .via
    if(readFileVia(arquivoVia, g, param) != 0){
        printf("[ERROR]\n");
        printf("In via.c [processarVia();]: Failed to read the .via file: %s\n", caminhoVia);
        fclose(arquivoVia);
        return -1;
    }

    // 4: Fecha o arquivo .via após o processamento
    fclose(arquivoVia);
    printf("\nArquivo .via processado com sucesso!\n");
    return 0;
}

Via* criarVia(){
    // 1: Aloca memória para a instância de Via
    Via* via = (Via*)malloc(sizeof(Via));
    // 2: Verifica se a alocação de memória foi bem-sucedida
    if(via == NULL){
        printf("[ERROR]\n");
        printf("In via.c [criarVia();]: Failed to allocate memory for Via instance\n");
        return NULL;
    }

    // 3: Inicializa os campos da estrutura Via
    via->nv      = 0;
    via->comando = NULL;
    via->id      = NULL;
    via->x       = 0.0;
    via->y       = 0.0;
    via->i       = NULL;
    via->j       = NULL;
    via->ldir    = NULL;
    via->lesq    = NULL;
    via->nome    = NULL;
    via->cmp     = 0.0;
    via->vm      = 0.0;

    // 4: Retorna o ponteiro para a instância de Via criada
    return via;
}

int freeVia(Via* via){
    // 1: Verifica se o ponteiro via é NULL
    if(via == NULL){
        printf("[ERROR]\n");
        printf("In via.c [freeVia();]: Via pointer is NULL\n");
        return -1;
    }

    // 2: Libera a memória alocada para os campos da estrutura Via
    free(via->comando);
    free(via->id);
    free(via->i);
    free(via->j);
    free(via->ldir);
    free(via->lesq);
    free(via->nome);

    // 3: Libera a memória alocada para a instância de Via
    free(via);

    // 4: Retorna 0 para indicar sucesso na liberação da memória
    return 0;
}
/*###############################################################################################*/