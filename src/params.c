#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
// Estrutura para armazenar os parâmetros/argumentos da linha de comando
typedef struct parametro{
    char* dirEntrada;            // Diretório de entrada (opcional)
    char* dirSaida;              // Diretório de saída   (obrigatório)
    char* nomeGeo;               // Nome do arquivo .geo (obrigatório)
    char* nomeQry;               // Nome do arquivo .qry (opcional)
    char* nomePM;                // Nome do arquivo .pm  (opcional)

    char* dirEntradaCompleto;    // Caminho completo do diretório de entrada
    char* dirSaidaCompleto;      // Caminho completo do diretório de saída
}Param;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int tratarCaminhosCompletos(Param* param){
    // Se o parâmetro -e não for fornecido, assume o diretório atual "./"

    /* ============================== DIRETÓRIO DE ENTRADA ============================== */
    // 1: Diretório de entrada
    if(param->dirEntrada != NULL){
        size_t lenDirEntrada = strlen(param->dirEntrada);

        // 1.1: Verifica se o diretório de entrada termina com '/' e, se não, 
        // adiciona '/' ao final do diretório de entrada para formar o caminho completo
        if(param->dirEntrada[lenDirEntrada - 1] != '/'){
            // 1.1.1: Aloca memória para o diretório de entrada completo, 
            // considerando o tamanho do diretório de entrada fornecido 
            // + 1 caractere para '/' + 1 caractere para o terminador nulo '\0'
            param->dirEntradaCompleto = (char*)malloc((lenDirEntrada + 2) * sizeof(char));
            if(param->dirEntradaCompleto == NULL){
                fprintf(stderr, "ERRO: Falha na alocação de memória para o diretório de entrada completo->\n");
                return -1;
            }

            // 1.1.2: Copia o diretório de entrada fornecido para o diretório de entrada completo e adiciona '/' ao final
            strcpy(param->dirEntradaCompleto, param->dirEntrada);
            strcat(param->dirEntradaCompleto, "/");
        }
        // 1.2: Se o diretório de entrada já terminar com '/', 
        // apenas faz uma cópia do diretório de entrada fornecido para o diretório de entrada completo
        else{
            param->dirEntradaCompleto = (char*)malloc((lenDirEntrada + 2) * sizeof(char));
            if(param->dirEntradaCompleto == NULL){
                fprintf(stderr, "ERRO: Falha na alocação de memória para o diretório de entrada completo.\n");
                return -1;
            }
            strcpy(param->dirEntradaCompleto, param->dirEntrada);
        }
    }
    // 2: Se o diretório de entrada não for fornecido, assume o diretório atual "./"
    else{
        param->dirEntradaCompleto = (char*)malloc(2 * sizeof(char) + 2);
        if(param->dirEntradaCompleto == NULL){
            fprintf(stderr, "ERRO: Falha na alocação de memória para o diretório de entrada completo.\n");
            return -1;
        }
        strcpy(param->dirEntradaCompleto, "./");
    }

    /* ============================== DIRETÓRIO DE SAÍDA ============================== */
    // 1: Diretório de saída
    if(param->dirSaida != NULL){
        size_t lenDirSaida = strlen(param->dirSaida);

        // 1.1: Verifica se o diretório de saída termina com '/' e, se não, 
        // adiciona '/' ao final do diretório de saída para formar o caminho completo
        if(param->dirSaida[lenDirSaida - 1] != '/'){
            // 1.1.1: Aloca memória para o diretório de saída completo, 
            // considerando o tamanho do diretório de saída fornecido + 1 caractere para '/'
            param->dirSaidaCompleto = (char*)malloc((lenDirSaida + 2) * sizeof(char));
            if(param->dirSaidaCompleto == NULL){
                fprintf(stderr, "ERRO: Falha na alocação de memória para o diretório de saída completo.\n");
                return -1;
            }

            // 1.1.2: Copia o diretório de saída fornecido para o diretório de saída completo e adiciona '/' ao final
            strcpy(param->dirSaidaCompleto, param->dirSaida);
            strcat(param->dirSaidaCompleto, "/");
        }
        // 1.2: Se o diretório de saída já terminar com '/', 
        // apenas faz uma cópia do diretório de saída fornecido para o diretório de saída completo
        else{
            param->dirSaidaCompleto = (char*)malloc((lenDirSaida + 2) * sizeof(char));
            if(param->dirSaidaCompleto == NULL){
                fprintf(stderr, "ERRO: Falha na alocação de memória para o diretório de saída completo.\n");
                return -1;
            }
            strcpy(param->dirSaidaCompleto, param->dirSaida);
        }
    }
    // 2: Se o diretório de saída não for fornecido, exibe uma mensagem de erro, pois o diretório de saída é obrigatório
    else{
        param->dirSaidaCompleto = (char*)malloc(2 * sizeof(char) + 2);
        if(param->dirSaidaCompleto == NULL){
            fprintf(stderr, "ERRO: Falha na alocação de memória para o diretório de saída completo.\n");
            return -1;
        }
        strcpy(param->dirSaidaCompleto, "./");
    }

    // 3: Exibe os caminhos completos dos arquivos e diretórios para debugação
    printf("Geo: \t\t%s\n", param->nomeGeo);
    printf("Qry: \t\t%s\n", param->nomeQry);
    printf("PM: \t\t%s\n", param->nomePM);
    printf("Dir Entrada: \t%s\n", param->dirEntradaCompleto);
    printf("Dir Saida: \t%s\n", param->dirSaidaCompleto);

    return 0;
}

int processarArgumentosInternos(Param* param, int argc, char* argv[]){
    // Parâmetros possíveis: -f (obrigatório), -o (obrigatório), -e (opcional), -q (opcional), -pm (opcional)
    // A ordem dos parâmetros pode variar
    // Exemplo de chamada: programa -f arquivo.geo -o dirSaida -e dirEntrada -q arquivo.qry -pm arquivo.pm

    // 1: Percorre os argumentos da linha de comando e processa os parâmetros
    int i = 1;
    while(i < argc){
        // 1.1: Parâmetro -f (nome do arquivo .geo)
        if(strcmp(argv[i], "-f") == 0){
            // 1.1.1: Verifica se o próximo argumento existe e não é outro parâmetro (começa com '-')
            if(argv[i + 1] != NULL){
                // Aloca memória para o nome do arquivo .geo
                param->nomeGeo = malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
                if(param->nomeGeo == NULL){
                    fprintf(stderr, "ERRO: Falha na alocação de memória para o nome do arquivo .geo.\n");
                    return -1;
                }

                // Copia o nome do arquivo .geo para o campo correspondente na estrutura de parâmetros
                strcpy(param->nomeGeo, argv[i + 1]);

                // Incrementa o índice para pular o nome do arquivo .geo e continuar processando os próximos argumentos
                i += 2;
            }
            // 1.1.2: Se o próximo argumento não existir ou for outro parâmetro, exibe uma mensagem de erro, pois o nome do arquivo .geo é obrigatório
            else{
                fprintf(stderr, "ERRO: Nome do arquivo .geo não fornecido. (-f obrigatório)\n");
                return -1;
            }
        }

        // 1.2: Parâmetro -o (diretório de saída)
        else if(strcmp(argv[i], "-o") == 0){
            // 1.2.1: Verifica se o próximo argumento existe e não é outro parâmetro (começa com '-')
            if(argv[i + 1] != NULL){
                // Aloca memória para o diretório de saída
                param->dirSaida = malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
                if(param->dirSaida == NULL){
                    fprintf(stderr, "ERRO: Falha na alocação de memória para o diretório de saída.\n");
                    return -1;
                }
                
                // Copia o diretório de saída para o campo correspondente na estrutura de parâmetros
                strcpy(param->dirSaida, argv[i + 1]);

                // Incrementa o índice para pular o diretório de saída e continuar processando os próximos argumentos
                i += 2;
            }
            // 1.2.2: Se o próximo argumento não existir ou for outro parâmetro, exibe uma mensagem de erro, pois o diretório de saída é obrigatório
            else{
                fprintf(stderr, "ERRO: Diretório de saída não fornecido. (-o obrigatório)\n");
                return -1;
            }
        }

        // 1.3: Parâmetro -e (diretório de entrada)
        else if(strcmp(argv[i], "-e") == 0){
            // 1.3.1: Verifica se o próximo argumento existe e não é outro parâmetro (começa com '-')
            if(argv[i + 1] != NULL){
                // Aloca memória para o diretório de entrada
                param->dirEntrada = malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
                if(param->dirEntrada == NULL){
                    fprintf(stderr, "ERRO: Falha na alocação de memória para o diretório de entrada.\n");
                    return -1;
                }
                
                // Copia o diretório de entrada para o campo correspondente na estrutura de parâmetros
                strcpy(param->dirEntrada, argv[i + 1]);

                // Incrementa o índice para pular o diretório de entrada e continuar processando os próximos argumentos
                i += 2;
            }
            // 1.3.2: Se o próximo argumento não existir ou for outro parâmetro, exibe uma mensagem de erro, pois o diretório de entrada é opcional
            else{
                fprintf(stderr, "ERRO: Diretório de entrada não fornecido. (-e opcional)\n");
                return -1;
            }
        }

        // 1.4: Parâmetro -q (nome do arquivo .qry)
        else if(strcmp(argv[i], "-q") == 0){
            // 1.4.1: Verifica se o próximo argumento existe e não é outro parâmetro (começa com '-')
            if(argv[i + 1] != NULL){
                // Aloca memória para o nome do arquivo .qry
                param->nomeQry = malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
                if(param->nomeQry == NULL){
                    fprintf(stderr, "ERRO: Falha na alocação de memória para o nome do arquivo .qry.\n");
                    return -1;
                }

                // Copia o nome do arquivo .qry para o campo correspondente na estrutura de parâmetros
                strcpy(param->nomeQry, argv[i + 1]);

                // Incrementa o índice para pular o nome do arquivo .qry e continuar processando os próximos argumentos
                i += 2;
            } 
            // 1.4.2: Se o próximo argumento não existir ou for outro parâmetro, exibe uma mensagem de erro, pois o nome do arquivo .qry é opcional
            else{
                fprintf(stderr, "ERRO: Nome do arquivo .qry não fornecido. (-q opcional)\n");
                return -1;
            }
        }

        // 1.5: Parâmetro -pm (nome do arquivo .pm)
        else if(strcmp(argv[i], "-pm") == 0){
            // 1.5.1: Verifica se o próximo argumento existe e não é outro parâmetro (começa com '-')
            if(argv[i + 1] != NULL){
                // Aloca memória para o nome do arquivo .pm
                param->nomePM = malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
                if(param->nomePM == NULL){
                    fprintf(stderr, "ERRO: Falha na alocação de memória para o nome do arquivo .pm.\n");
                    return -1;
                }
                
                // Copia o nome do arquivo .pm para o campo correspondente na estrutura de parâmetros
                strcpy(param->nomePM, argv[i + 1]);

                // Incrementa o índice para pular o nome do arquivo .pm e continuar processando os próximos argumentos
                i += 2;
            }
            // 1.5.2: Se o próximo argumento não existir ou for outro parâmetro, exibe uma mensagem de erro, pois o nome do arquivo .pm é opcional
            else{
                fprintf(stderr, "ERRO: Nome do arquivo .pm não fornecido. (-pm opcional)\n");
                return -1;
            }
        } 
        
        // 1.6: Se o argumento não for reconhecido como um parâmetro válido, exibe uma mensagem de aviso e ignora o argumento
        else{
            fprintf(stderr, "AVISO: Argumento desconhecido '%s' ignorado.\n", argv[i]);
            i++; // Garante que o loop não fique infinito
        }
    }

    return 0;
}

/**
 * Estas são as funções getters para os campos do objeto de parâmetros. 
 * Elas são usadas para acessar os valores dos campos do objeto de parâmetros a partir de outras partes do programa, 
 * como as funções de processamento dos arquivos .geo, .qry e .pm.
 */
char* getDirEntradaCompleto(Param* param) {return param->dirEntradaCompleto;}
char* getDirSaidaCompleto  (Param* param) {return param->dirSaidaCompleto;  }
char* getDirEntrada        (Param* param) {return param->dirEntrada;        }
char* getNomeGeo           (Param* param) {return param->nomeGeo;           }
char* getNomeQry           (Param* param) {return param->nomeQry;           }
char* getNomePM            (Param* param) {return param->nomePM;            }
void  setDirEntradaCompleto(Param* param, const char* dirEntradaCompleto){
    // 1: Verifica se o campo dirEntradaCompleto já possui um valor alocado
    // e, se sim, libera a memória alocada para evitar vazamento de memória 
    // antes de atribuir um novo valor ao campo dirEntradaCompleto
    if(param->dirEntradaCompleto != NULL) free(param->dirEntradaCompleto);

    // 2: Aloca memória para o campo dirEntradaCompleto
    param->dirEntradaCompleto = malloc(sizeof(char) * (strlen(dirEntradaCompleto) + 1));
    if(param->dirEntradaCompleto == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o diretorio de entrada completo.\n");
        return;
    }

    // 3: Copia o valor do diretório de entrada completo para o campo dirEntradaCompleto da estrutura de parâmetros
    strcpy(param->dirEntradaCompleto, dirEntradaCompleto);
}
void  setDirEntrada(Param* param, const char* dirEntrada){
    // 1: Verifica se o campo dirEntrada já possui um valor alocado
    // e, se sim, libera a memória alocada para evitar vazamento de memória 
    // antes de atribuir um novo valor ao campo dirEntrada
    if(param->dirEntrada != NULL) free(param->dirEntrada);

    // 2: Aloca memória para o campo dirEntrada
    param->dirEntrada = malloc(sizeof(char) * (strlen(dirEntrada) + 1));
    if(param->dirEntrada == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o diretorio de entrada.\n");
        return;
    }

    // 3: Copia o valor do diretório de entrada para o campo dirEntrada da estrutura de parâmetros
    strcpy(param->dirEntrada, dirEntrada);
}
void  setDirSaida(Param* param, const char* dirSaida){
    // 1: Verifica se o campo dirSaida já possui um valor alocado
    // e, se sim, libera a memória alocada para evitar vazamento de memória 
    // antes de atribuir um novo valor ao campo dirSaida
    if(param->dirSaida != NULL) free(param->dirSaida);

    // 2: Aloca memória para o campo dirSaida
    param->dirSaida = malloc(sizeof(char) * (strlen(dirSaida) + 1));
    if(param->dirSaida == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o diretorio de saida.\n");
        return;
    }

    // 3: Copia o valor do diretório de saída para o campo dirSaida da estrutura de parâmetros
    strcpy(param->dirSaida, dirSaida);
}
void  setNomeGeo(Param* param, const char* nomeGeo){
    // 1: Verifica se o campo nomeGeo já possui um valor alocado
    // e, se sim, libera a memória alocada para evitar vazamento de memória 
    // antes de atribuir um novo valor ao campo nomeGeo
    if(param->nomeGeo != NULL) free(param->nomeGeo);

    // 2: Aloca memória para o campo nomeGeo
    param->nomeGeo = malloc(sizeof(char) * (strlen(nomeGeo) + 1));
    if(param->nomeGeo == NULL){
        fprintf(stderr, "ERRO: Falha na alocação de memória para o nome do arquivo .geo.\n");
        return;
    }

    // 3: Copia o valor do nome do arquivo .geo para o campo nomeGeo da estrutura de parâmetros
    strcpy(param->nomeGeo, nomeGeo);
}
void  setNomeQry(Param* param, const char* nomeQry){
    // 1: Verifica se o campo nomeQry já possui um valor alocado
    // e, se sim, libera a memória alocada para evitar vazamento de memória 
    // antes de atribuir um novo valor ao campo nomeQry
    if(param->nomeQry != NULL) free(param->nomeQry);

    // 2: Aloca memória para o campo nomeQry
    param->nomeQry = malloc(sizeof(char) * (strlen(nomeQry) + 1));
    if(param->nomeQry == NULL){
        fprintf(stderr, "ERRO: Falha na alocação de memória para o nome do arquivo .qry.\n");
        return;
    }

    // 3: Copia o valor do nome do arquivo .qry para o campo nomeQry da estrutura de parâmetros
    strcpy(param->nomeQry, nomeQry);
}
void  setNomePM(Param* param, const char* nomePM){
    // 1: Verifica se o campo nomePM já possui um valor alocado
    // e, se sim, libera a memória alocada para evitar vazamento de memória 
    // antes de atribuir um novo valor ao campo nomePM
    if(param->nomePM != NULL) free(param->nomePM);

    // 2: Aloca memória para o campo nomePM
    param->nomePM = malloc(sizeof(char) * (strlen(nomePM) + 1));
    if(param->nomePM == NULL){
        fprintf(stderr, "ERRO: Falha na alocação de memória para o nome do arquivo .pm.\n");
        return;
    }

    // 3: Copia o valor do nome do arquivo .pm para o campo nomePM da estrutura de parâmetros
    strcpy(param->nomePM, nomePM);
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
Param* criarParametro(){
    // 1: Aloca memória para o objeto de parâmetros
    Param* param = (Param*)malloc(sizeof(Param));
    if(param == NULL){
        fprintf(stderr, "Erro: Falha na alocacao de memoria para o objeto Parametro.\n");
        return NULL;
    }

    // 2: Inicializa os campos do objeto de parâmetros com NULL ou valores padrão
    param->dirEntrada = NULL;
    param->dirSaida   = NULL;
    param->nomeGeo    = NULL;
    param->nomeQry    = NULL;
    param->nomePM     = NULL;

    param->dirEntradaCompleto = NULL;
    param->dirSaidaCompleto   = NULL;

    // 3: Retorna o ponteiro para o objeto de parâmetros criado
    return param;
}

int processarParametros(Param* param, int argc, char* argv[]){
    // Mínimo de argumentos da linha de comando: 5
    // Exemplo: -f arquivo.geo -o dirSaida (-f e -o são obrigatórios)

    // 1: Verifica se o número mínimo de argumentos foi fornecido
    if(argc < 5){
        fprintf(stderr, "ERRO: Numero insuficiente de argumentos.\n");
        return -1;
    }

    // 2: Processa os argumentos da linha de comando 
    // e armazena as informações necessárias para a execução do programa no objeto de parâmetros criado na etapa anterior
    if(processarArgumentosInternos(param, argc, argv) == -1){
        printf("ERRO: Erro ao processar os argumentos da linha de comando.\n");
        return -1;
    }

    // 3: Trata os caminhos completos dos arquivos e diretórios, garantindo que estejam no formato correto para a execução do programa
    if(tratarCaminhosCompletos(param) == -1){
        printf("ERRO: Erro ao tratar os caminhos completos dos arquivos e diretorios.\n");
        return -1;
    }

    return 0;
}

int freeParametros(Param* param){    
    // 1: Verifica se o ponteiro para o objeto de parâmetros é NULL 
    // antes de tentar liberar a memória alocada para evitar erros de segmentação
    if(param == NULL) return -1;
    
    printf("\nLiberando parametros...\n");

    // 2: Libera a memória alocada para os campos do objeto Parametro, se não forem NULL
    if(param->dirEntrada != NULL) free(param->dirEntrada);
    if(param->nomeGeo    != NULL) free(param->nomeGeo);
    if(param->nomeQry    != NULL) free(param->nomeQry);
    if(param->nomePM     != NULL) free(param->nomePM);
    if(param->dirSaida   != NULL) free(param->dirSaida);

    if(param->dirEntradaCompleto != NULL) free(param->dirEntradaCompleto);
    if(param->dirSaidaCompleto   != NULL) free(param->dirSaidaCompleto);

    // 3: Libera a memória alocada para o objeto Parametro
    free(param);
    param = NULL;

    printf("Parametros liberados com sucesso!\n");
    return 0;
}
/*###############################################################################################*/
