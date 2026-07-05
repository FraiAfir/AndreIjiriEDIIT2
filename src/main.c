#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "program.h"
#include "hashBin.h"
#include "geo.h"
#include "qry.h"
#include "via.h"
#include "svg.h"

int main(int argc, char* argv[]){
    printf("##################### INICIO DA EXECUCAO DO PROGRAMA #####################\n\n");

    printf("Argumentos da linha de comando:\n");
    for(int i = 0; i < argc; i++) {printf("Argumento do argv[%d]: %s\n", i, argv[i]);}

    // 1: Cria os objetos das estruturas necssárias para a execução do código
    Param   *param = criarParametro();
    HashBin *h     = NULL;
    Quadras *q     = criarQuadra();
    Grafo   *g     = NULL;



    // 2. PROCESSAR PARÂMETROS DA LINHA DE COMANDOS
    printf("\n\n\n\n\n#------------ PROCESSANDO OS PARAMETROS DA LINHA DE COMANDO... ----------#\n");
    // 2.1: Processa os parâmetros da linha de comando e armazena as informações necessárias para a execução do programa no objeto de parâmetros criado na etapa anterior
    if(processarParametros(param, argc, argv) == -1){
        printf("[ERROR]\n");
        printf("In main.c [processarParametros();]: Error processing command line parameters.\n");
        shutProgram(&param, &h, &q, &g);
        return -1;
    }printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    // 2.2: Cria a estrutura de dados necessária para armazenar os dados do arquivo .geo
    h = criarHash(param);



    // 3. PROCESSAR O GEO
    printf("\n#-------------------- PROCESSANDO O ARQUIVO .GEO... ---------------------#\n");
    // 3.1: Processa o arquivo .geo e armazena os dados em uma estrutura de dados adequada (Tabela Hash, Quadras, etc.)
    if(processarGeo(param, h, q) != 0){
        printf("[ERROR]\n");
        printf("In main.c [processarGeo();]: Error processing .geo file.\n");
        shutProgram(&param, &h, &q, &g);
        return -1;
    }printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    
    // 3.2: Salva o diretório da tabela hash com os dados do arquivo .geo em um arquivo de saída no formato .hfc
    if(salvarDiretorioHFC(h, param) != 0){
        printf("[ERROR]\n");
        printf("In main.c [salvarDiretorioHFC();]: Error saving HFC directory.\n");
        shutProgram(&param, &h, &q, &g);
        return -1;
    }



    // 4. PROCESSAR O QRY (Se fornecido)
    // 4.1: Verifica se o arquivo .qry foi fornecido como argumento.
    if (getNomeQry(param) == NULL){
        printf("\n#----------- ARQUIVO .QRY NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n\n");
    }else{
        printf("\n#-------------------- PROCESSANDO O ARQUIVO .QRY... ---------------------#\n");
        // 4.1: Processa o arquivo .qry utilizando os dados armazenados a partir do processamento do arquivo .geo (e do arquivo .pm, se fornecido)
        if(processarQry(param, h) != 0){
            printf("[ERROR]\n");
            printf("In main.c [processarQry();]: Error processing .qry file.\n");
            shutProgram(&param, &h, &q, &g);
            return -1;
        }
        printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    }



    // 5. PROCESSAR O VIA (Se fornecido)
    // 5.1: Verifica se o arquivo .via foi fornecido como argumento. 
    // Se não for fornecido, exibe uma mensagem informando que esta etapa será pulada.
    if(getNomeVia(param) == NULL){
        printf("\n#----------- ARQUIVO .VIA NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    }else{
        // Se o arquivo .via for fornecido, o processa e armazena os dados em uma estrutura de dados adequada (Grafo)
        printf("\n#--------------------- PROCESSANDO O ARQUIVO .VIA... --------------------#\n");
        // 5.2: Processa o arquivo .via e armazena os dados em uma estrutura de dados adequada (Tabela Hash, Pessoas, etc.)
        if(processarVia(param, g) != 0){
            printf("[ERROR]\n");
            printf("In main.c [processarVia();]: Error processing .via file.\n");
            shutProgram(&param, &h, &q, &g);
            return -1;
        }
    }
    printf("#------------------------------------------------------------------------#\n\n\n\n\n");

    
    
    // 5: LIBERAR MEMÓRIA ALOCADA PARA PARÂMETROS E ENCERRAR PROGRAMA
    printf("#---------------------- ENCERRANDO O PROGRAMA... ------------------------#\n");
    shutProgram(&param, &h, &q, &g);
    printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    printf("\n##################### FIM DA EXECUCAO DO PROGRAMA ########################\n\n");

    printf("\n");
    return 0;
}