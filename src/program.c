#include <stdio.h>
#include <stdlib.h>

#include "program.h"

/*                                       FUNÇÕES PRINCIPAIS                                      */

int shutProgram(Param** param, HashBin** h, Quadras** q, Grafo** g){
    printf("\n\n");
    printf("Freeing allocated resources...\n");

    // 1: Libera a memória alocada para os objetos de Parametro
    if(*param == NULL){
        printf("[WARNING]\n");
        printf("In program.c [shutProgram();]: Parametro is NULL, nothing to free\n\n");
    }else{
        freeParametros(*param);
        *param = NULL;
        printf("[INFO]\n");
        printf("In program.c [shutProgram();]: Parametro freed successfully.\n\n");
    }

    // 2: Libera a tabela hash criada para armazenar os dados do arquivo .geo
    if(*h == NULL){
        printf("[WARNING]\n");
        printf("In program.c [shutProgram();]: HashBin is NULL, nothing to free\n\n");
    }else{
        freeHash(*h);
        *h = NULL;
        printf("[INFO]\n");
        printf("In program.c [shutProgram();]: HashBin freed successfully.\n\n");
    }

    // 3: Libera Quadra criada para armazenar os dados do arquivo .geo
    if(*q == NULL){
        printf("[WARNING]\n");
        printf("In program.c [shutProgram();]: Quadras is NULL, nothing to free\n\n");
    }else{
        freeQuadra(*q);
        *q = NULL;
        printf("[INFO]\n");
        printf("In program.c [shutProgram();]: Quadras freed successfully.\n\n");
    }

    // 4: Libera a estrutura de dados utilizada para armazenar os dados do arquivo .via
    if(*g == NULL){
        printf("[WARNING]\n");
        printf("In program.c [shutProgram();]: Grafo is NULL, nothing to free\n\n");
    }else{
        freeGrafo(*g);
        *g = NULL;
        printf("[INFO]\n");
        printf("In program.c [shutProgram();]: Grafo freed successfully.\n\n");
    }

    printf("\n");
    printf("All allocated resources have been freed successfully.\n");
    return 0;
}
/*###############################################################################################*/