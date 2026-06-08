#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "program.h"
#include "hashBin.h"
#include "geo.h"
#include "qry.h"

int main(int argc, char* argv[]){
    system("cls");

    printf("##################### INICIO DA EXECUCAO DO PROGRAMA #####################\n\n");

    printf("Argumentos da linha de comando:\n");
    for(int i = 0; i < argc; i++) {printf("Argumento do argv[%d]: %s\n", i, argv[i]);}

    // 1: Cria os objetos das estruturas necssárias para a execução do código
    Param   *param  = NULL;
    HashBin *htq    = NULL;
    Quadras *q      = NULL;

    // 1.1: Inicializa os objetos de Parametro e da ED para armazenar os dados do arquivo .geo
    if(bootProgram(&param, &htq, &q) == -1){
        printf("ERRO: Inicializacao dos objetos.\n");
        return -1;
    }

    // 2. PROCESSAR PARÂMETROS DA LINHA DE COMANDOS
    printf("\n\n\n\n\n#------------ PROCESSANDO OS PARAMETROS DA LINHA DE COMANDO... ----------#\n");
    // 2.1: Processa os parâmetros da linha de comando e armazena as informações necessárias para a execução do programa no objeto de parâmetros criado na etapa anterior
    if(processarParametros(param, argc, argv) == -1) {
        printf("ERRO: Processamento dos parametros da linha de comando.\n");
        shutProgram(&param, &htq, &q);
        return -1;
    }printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    

    // 3. PROCESSAR O GEO
    printf("\n#-------------------- PROCESSANDO O ARQUIVO .GEO... ---------------------#\n");
    // 3.1: Processa o arquivo .geo e armazena os dados em uma estrutura de dados adequada (Tabela Hash, Quadras, etc.)
    if(processarGeo(param, htq, q) != 0){
        printf("ERRO: Processamento do arquivo .geo.\n");
        shutProgram(&param, &htq, &q);
        return -1;
    }printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    
    // 3.2: Salva o diretório da tabela hash com os dados do arquivo .geo em um arquivo de saída no formato .hfc
    if(salvarDiretorioHFC(htq, getNomeGeo(param)) != 0){
        printf("ERRO: Salvamento do diretório da tabela hash do arquivo .geo.\n");
        shutProgram(&param, &htq, &q);
        return -1;
    }
    printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    
    // // 4. PROCESSAR O PM (Se fornecido)
    // // 4.1: Verifica se o arquivo .pm foi fornecido como argumento. 
    // // Se sim, processa o arquivo .pm e armazena os dados em uma estrutura de dados adequada (Tabela Hash, Pessoas, etc.)
    // if(getNomePM(param) == NULL){
    //     printf("\n#----------- ARQUIVO .PM NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    // }else{
    //     printf("\n#--------------------- PROCESSANDO O ARQUIVO .PM... --------------------#\n");
    //     // 4.2: Processa o arquivo .pm e armazena os dados em uma estrutura de dados adequada (Tabela Hash, Pessoas, etc.)
    //     if(processarPM(param, htp, htq, p) != 0){
    //         printf("ERRO: Processamento do arquivo .pm.\n");
    //         shutProgram(&param, &htq, &q, &htp, &p);
    //         return -1;
    //     }

    //     // 4.3: Salva o diretório da tabela hash com os dados do arquivo .pm em um arquivo de saída no formato .hfc
    //     if(salvarDiretorioHFC_PM(htp, getNomePM(param)) != 0){
    //         printf("ERRO: Salvamento do diretório da tabela hash do arquivo .pm.\n");
    //         shutProgram(&param, &htq, &q, &htp, &p);
    //         return -1;
    //     }
        
    //     // 4.4: Gera um relatório com os dados do arquivo .pm em um arquivo de saída no formato .hfd
    //     if(gerarRelatorioHFD(htp, getNomePM(param)) != 0){
    //         printf("ERRO: Geracao do relatorio .hfd do arquivo .pm.\n");
    //         shutProgram(&param, &htq, &q, &htp, &p);
    //         return -1;
    //     }
    // }
    // printf("#------------------------------------------------------------------------#\n\n\n\n\n");

    // 4. PROCESSAR O QRY (Se fornecido)
    // 4.1: Verifica se o arquivo .qry foi fornecido como argumento.
    if (getNomeQry(param) == NULL){
        printf("\n#----------- ARQUIVO .QRY NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n\n");
    }else{
        printf("\n#-------------------- PROCESSANDO O ARQUIVO .QRY... ---------------------#\n");
        // 4.1: Processa o arquivo .qry utilizando os dados armazenados a partir do processamento do arquivo .geo (e do arquivo .pm, se fornecido)
        if(processarQry(param, htq) != 0){
            printf("ERRO: Processamento do arquivo .qry.\n");
            shutProgram(&param, &htq, &q);
            return -1;
        }
        printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    }
    
    // 5: LIBERAR MEMÓRIA ALOCADA PARA PARÂMETROS E ENCERRAR PROGRAMA
    printf("#---------------------- ENCERRANDO O PROGRAMA... ------------------------#\n");
    shutProgram(&param, &htq, &q);
    printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    printf("\n##################### FIM DA EXECUCAO DO PROGRAMA ########################\n\n");

    return 0;
}