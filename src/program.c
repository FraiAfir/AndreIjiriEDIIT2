#include <stdio.h>
#include <stdlib.h>

#include "program.h"

/*                                       FUNÇÕES PRINCIPAIS                                      */
int bootProgram(Param** param, HashBin** htq, Quadras** q){
    // 1: Cria o objeto de parâmetros para armazenar os dados dos argumentos da linha de comando
    *param = criarParametro();
    if(*param == NULL){
        printf("ERRO: Falha na alocacao de memoria para o objeto Parametro\n");
        shutProgram(param, htq, q);
        return -1;
    }

    // 2: Cria a estrutura de dados necessária para armazenar os dados do arquivo .geo
    *htq = criarHash(*param);

    // 2.3: Verifica se a tabela hash foi criada com sucesso
    if(*htq == NULL){
        fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .geo.\n");
        shutProgram(param, htq, q);
        return -1;
    }
    
    // // 3: Cria a estrutura de dados necessária para armazenar os dados do arquivo .pm
    // printf("\tCriando a Tabela Hash para armazenar os dados do arquivo .pm...\n");
    // *hp = criarHashPM("pessoas.hf");
    // if(*hp == NULL){
    //     fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .pm.\n");
    //     shutProgram(param);
    //     return -1;
    // }printf("\tTabela Hash criada com sucesso para armazenar os dados do arquivo .pm\n");

    // 4: Criar o objeto para armazenar os dados lidos do arquivo .geo
    *q = criarQuadra();
    if(*q == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Quadras\n");
        shutProgram(param, htq, q);
        return -1;
    }
    
    // // 5: Criar o objeto para armazenar os dados lidos do arquivo .pm
    // printf("\tCriando o objeto de Pessoas para armazenar os dados do arquivo .pm...\n");
    // *p = criarPessoa();
    // if(*p == NULL){
    //     fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Pessoas\n");
    //     shutProgram(param, htq, q, hp, p);
    //     return -1;
    // }printf("\tObjeto de Pessoas criado com sucesso para armazenar os dados do arquivo .pm\n");

    return 0;
}

int shutProgram(Param** param, HashBin** htq, Quadras** q){
    // 1: Libera a memória alocada para os objetos de Parametro
    if(*param != NULL){
        freeParametros(*param);
        *param = NULL;
    }else printf("\tObjeto de Parametro nao alocado. Nenhuma memoria para liberar.\n");

    // 2: Libera a estrutura de dados utilizada
    freeHash(*htq);

    // 3: Libera a memória alocada para o objeto de Quadras
    freeQuadra(*q);

    // // 4: Libera a estrutura de dados utilizada para armazenar os dados do arquivo .pm
    // freeHashPM(*hp);

    // // 5: Libera a memória alocada para o objeto de Pessoas
    // freePessoa(*p);

    return 0;
}
/*###############################################################################################*/