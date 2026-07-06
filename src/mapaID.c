#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mapaID.h"



/*                             ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                          */
typedef struct entrada{
    char id[64];
    int indiceVetor;
    int flag; // 0 = vazio | 1 = ocupado
} Entrada;
typedef struct mapa{
    Entrada* hash;
    int capacidade;
} Mapa;
/*################################################################################################*/



unsigned long hashFuncString(const char *str){
    unsigned long hash = 5381;
    int c;

    while((c = *str++)) {hash = ((hash << 5) + hash) + c;}

    return hash;
}

Mapa* criarMapa(int capacidade){
    // 1: Aloca memória para a estrutura Mapa
    Mapa* mapa = (Mapa*)malloc(sizeof(Mapa));

    // 2: Verifica se a alocação foi bem-sucedida
    if(mapa == NULL){
        printf("[ERROR]\n");
        printf("In mapaID.c [criarMapa();]: Memory allocation unsuccessful for Mapa structure.\n");
        return NULL;
    }

    // 3: Inicializa a capacidade do mapa e aloca memória para o vetor de entradas (hash)
    // 3.1: Multiplicamos a capacidade por 2 para evitar fator de carga alto (muitas colisões)
    mapa->capacidade = capacidade * 2;
    // 3.2: Aloca memória para o vetor de entradas (hash)
    mapa->hash = (Entrada*)calloc(mapa->capacidade, sizeof(Entrada));
    // 3.3: Verifica se a alocação foi bem-sucedida
    if(mapa->hash == NULL){
        printf("[ERROR]\n");
        printf("In mapaID.c [criarMapa();]: Memory allocation unsuccessful for hash table.\n");
        free(mapa);
        return NULL;
    }

    // 4: Inicializa todos os campos como vazios
    for(int i = 0; i < mapa->capacidade; i++) {mapa->hash[i].flag = 0;}

    // 5: Retorna o ponteiro para a estrutura Mapa criada
    return mapa;
}

int inserirMapa(Mapa* mapa, char* id, int indiceVetor){
    // 1: Verifica se o mapa é nulo
    if(mapa == NULL){
        printf("[ERROR]\n");
        printf("In mapaID.c [inserirMapa();]: Mapa is NULL.\n");
        return -1;
    }

    // 2: Calcula o índice usando a função de hash e o tamanho da tabela
    // Ex: Se a capacidade for 10, e o hashFuncString(id) retornar 123456 = 123456 % 10 = 6
    unsigned long indice = hashFuncString(id) % mapa->capacidade;

    // 3: Procura uma posição livre na tabela (tratamento de colisões por sondagem linear)
    while(mapa->hash[indice].flag){
        // 3.1: Se for o mesmo ID, atualiza (segurança para evitar duplicatas)
        if(strcmp(mapa->hash[indice].id, id) == 0){
            mapa->hash[indice].indiceVetor = indiceVetor;
            return 0;
        }
        // 3.2: Move para a próxima posição (sondagem linear)
        // Ex: Se a capacidade for 10, e o índice atual for 6, a próxima posição será (6 + 1) % 10 = 7
        indice = (indice + 1) % mapa->capacidade;
    }

    // 4: Posição livre encontrada, insere o ID e o índice do vetor
    strcpy(mapa->hash[indice].id, id);
    mapa->hash[indice].indiceVetor = indiceVetor;
    mapa->hash[indice].flag = 1;

    return 0;
}

int buscarMapa(Mapa* mapa, char* id){
    // 1: Verifica se o mapa é nulo
    if(mapa == NULL){
        printf("[ERROR]\n");
        printf("In mapaID.c [buscarMapa();]: Mapa is NULL.\n");
        return -1;
    }

    // 2: Calcula o índice usando a função de hash e o tamanho da tabela
    unsigned long indice = hashFuncString(id) % mapa->capacidade;

    // 3: Procura na tabela
    while(mapa->hash[indice].flag){
        // 3.1: Se o ID for encontrado, retorna o índice do vetor
        if(strcmp(mapa->hash[indice].id, id) == 0) {return mapa->hash[indice].indiceVetor;}
        // 3.2: Move para a próxima posição (sondagem linear)
        // Ex: Se a capacidade for 10, e o índice atual for 6, a próxima posição será (6 + 1) % 10 = 7
        indice = (indice + 1) % mapa->capacidade;
    }

    // 4: Se não encontrou o ID
    printf("In mapaID.c [buscarMapa();]: ID not found in the map.\n");
    return -1;
}

int destruirMapa(Mapa* mapa){
    // 1: Verifica se o mapa é nulo
    if(mapa == NULL){
        printf("[ERROR]\n");
        printf("In mapaID.c [destruirMapa();]: Mapa is NULL.\n");
        return -1;
    }

    // 2: Libera a memória alocada para o vetor de entradas (hash) e para a estrutura Mapa
    free(mapa->hash);
    free(mapa);

    // 3: Retorna 0 para indicar sucesso
    return 0;
}