#include <stdio.h>
#include <stdlib.h>

#include "program.h"
#include "params.h"

/*                                       FUNÇÕES PRINCIPAIS                                      */
int bootProgram(Param** param){
    printf("Inicilizando os objetos e alocando os recursos necessarios:\n");

    // 1: Cria o objeto de parâmetros para armazenar os dados dos argumentos da linha de comando
    printf("\tCriando o objeto de Parametro para armazenar os dados dos argumentos da linha de comando...\n");
    *param = criarParametro();
    if(*param == NULL){
        printf("ERRO: Falha na alocacao de memoria para o objeto Parametro\n");
        shutProgram(param);
        return -1;
    }printf("\tObjeto de Parametro criado com sucesso para armazenar os dados dos argumentos da linha de comando\n");

    printf("\n");

    // // 2: Cria a estrutura de dados necessária para armazenar os dados do arquivo .geo
    // printf("\tCriando a Tabela Hash para armazenar os dados do arquivo .geo...\n");
    // *ht = criarHash("quadras.hf");
    // if(*ht == NULL){
    //     fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .geo.\n");
    //     shutProgram(param);
    //     return -1;
    // }printf("\tTabela Hash criada com sucesso para armazenar os dados do arquivo .geo\n");

    // printf("\n");

    // // 3: Cria a estrutura de dados necessária para armazenar os dados do arquivo .pm
    // printf("\tCriando a Tabela Hash para armazenar os dados do arquivo .pm...\n");
    // *hp = criarHashPM("pessoas.hf");
    // if(*hp == NULL){
    //     fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .pm.\n");
    //     shutProgram(param);
    //     return -1;
    // }printf("\tTabela Hash criada com sucesso para armazenar os dados do arquivo .pm\n");

    // printf("\n");

    // // 4: Criar o objeto para armazenar os dados lidos do arquivo .geo
    // printf("\tCriando o objeto de Quadras para armazenar os dados do arquivo .geo...\n");
    // *q = criarQuadra();
    // if(*q == NULL){
    //     fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Quadras\n");
    //     shutProgram(param, ht, q, hp, NULL);
    //     return -1;
    // }printf("\tObjeto de Quadras criado com sucesso para armazenar os dados do arquivo .geo\n");

    // printf("\n");

    // // 5: Criar o objeto para armazenar os dados lidos do arquivo .pm
    // printf("\tCriando o objeto de Pessoas para armazenar os dados do arquivo .pm...\n");
    // *p = criarPessoa();
    // if(*p == NULL){
    //     fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Pessoas\n");
    //     shutProgram(param, ht, q, hp, p);
    //     return -1;
    // }printf("\tObjeto de Pessoas criado com sucesso para armazenar os dados do arquivo .pm\n");

    printf("Objetos e recursos alocados com sucesso!\n");
    return 0;
}

int shutProgram(Param** param){
    printf("Encerrando o programa e liberando os recursos alocados...\n");

    // 1: Libera a memória alocada para os objetos de Parametro
    if(*param != NULL){
        freeParametro(*param);
        *param = NULL;
        printf("\tMemoria alocada para o objeto de Parametro liberada com sucesso!\n");
    }else printf("\tObjeto de Parametro nao alocado. Nenhuma memoria para liberar.\n");

    // // 2: Libera a estrutura de dados utilizada
    // freeHash(*ht);

    // // 3: Libera a memória alocada para o objeto de Quadras
    // freeQuadra(*q);

    // // 4: Libera a estrutura de dados utilizada para armazenar os dados do arquivo .pm
    // freeHashPM(*hp);

    // // 5: Libera a memória alocada para o objeto de Pessoas
    // freePessoa(*p);

    printf("\nPrograma encerrado com sucesso!\n");
    return 0;
}
/*###############################################################################################*/