#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"
// #include "svg.h"

/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoGeo(Param* param, char caminhoGeo[512]){
    // 1: Obtém o diretório de entrada completo e o nome do arquivo .geo a partir da estrutura de parâmetros
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomeGeo    = getNomeGeo(param);
    printf("Diretorio de entrada: \t\t\t\t%s\n", dirEntrada);
    printf("Arquivo .geo fornecido: \t\t\t%s\n", nomeGeo);
    
    // 2: Concatena o diretório de entrada completo com o nome do arquivo .geo
    strcpy(caminhoGeo, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoGeo, nomeGeo);    // Concatena o nome do arquivo .geo ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .geo: \t\t%s\n", caminhoGeo);

    return 0;
}

int readFileGeo(FILE* arquivoGeo, HashBin* dir, Quadras* q, Param* param){
    // 1: PRÉ-LEITURA (Descobrir o tamanho máximo de Bitnópolis)
    // 1.1: Buffers para armazenar os dados lidos de cada linha do arquivo .geo
    char linha[256];
    char comando[5];

    // 1.2: Variáveis para armazenar as dimensões máximas da cidade (Para definir o tamanho do arquivo .svg)
    double max_x = 0.0;
    double max_y = 0.0;

    // 1.3: Lê o arquivo .geo linha por linha para descobrir as dimensões máximas da cidade
    while(fgets(linha, sizeof(linha), arquivoGeo)){
        // 1.3.1: Se a linha estiver vazia ou não contiver um comando, pula para a próxima iteração
        if(sscanf(linha, "%s", comando) != 1) continue; 

        // 1.3.2: Verifica o comando lido e atualiza as dimensões máximas da cidade com base nos dados das quadras (Comando 'q')
        if(strcmp(comando, "q") == 0){
            char cep_tmp[20];
            double x, y, w, h;
            // Lê apenas para pegar as dimensões
            sscanf(linha, "q %s %lf %lf %lf %lf", cep_tmp, &x, &y, &w, &h);
            
            // O extremo da quadra é o seu (X + Largura) e (Y + Altura)
            if((x + w) > max_x) max_x = x + w;
            if((y + h) > max_y) max_y = y + h;
        }
    }
    // 1.4: "Rebobina" o ponteiro de leitura do arquivo .geo de volta para a primeira linha
    rewind(arquivoGeo);

    // 2: Cria o arquivo .svg para escrita do conteúdo do arquivo .geo
    char* geoSVG = getNomeGeo(param);
    char* ponto = strchr(geoSVG, '.');
    if(ponto != NULL) *ponto = '\0';
    strcat(geoSVG, ".svg");
    // FILE* arqSvg = criarSvg(geoSVG, max_x, max_y);
    // if(arqSvg == NULL) {
    //     fprintf(stderr, "ERRO: Não foi possível criar o SVG.\n");
    //     return -1;
    // }

    // 3: Variáveis temporárias para armazenar os dados lidos de cada linha do arquivo .geo
    char sw[256] = "1.0px";     // Largura da borda padrão
    char cfill[256] = "white";  // Cor de preenchimento padrão
    char cstrk[256] = "black";  // Cor da borda padrão
    
    // 4: Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoGeo) != NULL){

        // 4.1: Lê o comando da linha para determinar o tipo de dado a ser processado
        sscanf(linha, "%s", comando);

        // 4.2: Processa o comando lido do arquivo .geo
        if(strcmp(comando, "cq") == 0){
            // Comando cq - Define a largura da borda(sw), a cor de preenchimento (cfill) e a cor da borda (cstrk) para os próximos comandos q
            // '*' é usado para ignorar a primeira string (cq) e ler os próximos valores
            // Exemplo de linha: comando  sw   cfill cstrk
            // Exemplo de linha:   cq    2.0px  red   blue
            sscanf(linha, "%*s %s %s %s", sw, cfill, cstrk);
        }else if(strcmp(comando, "q") == 0){
            // Comando q - Insere uma quadra (retângulo e CEP)
            // '*' é usado para ignorar a primeira string (q) e ler os próximos valores
            // Exemplo de linha: comando cep  x    y    w    h
            // Exemplo de linha:    q    Q1 10.0 20.0 30.0 40.0
            char cep[5];
            double x, y, w, h;
            sscanf(linha, "%*s %s %lf %lf %lf %lf", cep, &x, &y, &w, &h);

            // 4.2.1: Insere a quadra na estrutura de dados (Tabela Hash e Quadras)
            inserirReg(dir, cep, x, y, w, h, sw, cfill, cstrk);

            // 4.2.2: Desenha a quadra no arquivo SVG
            // desenharFormaSvg(arqSvg, "r", x, y, w, h, sw, cstrk, cfill);

            /**
             * Escreve o CEP da quadra no canto superior esquerdo da quadra no arquivo SVG
             * Tamanho da fonte 10 para legibilidade
             * Peso da fonte negrito para destacar o CEP
             */
            // 4.2.3: Se a cor de preenchimento for branca, use preto para o texto do CEP para garantir legibilidade
            // if(strcmp(cfill, "white") == 0) 
                // fprintf(arqSvg, "\t<text x=\"%lf\" y=\"%lf\" font-size=\"10\" font-weight=\"bold\" fill=\"%s\">%s</text>\n", x + 1, y + 11, "black", cep);

            // 4.2.4: Se não, use branco para o texto do CEP para garantir legibilidade
            // else
            //     fprintf(arqSvg, "\t<text x=\"%lf\" y=\"%lf\" font-size=\"10\" font-weight=\"bold\" fill=\"%s\">%s</text>\n", x + 1, y + 11, "white", cep);
        }
    }

    // 5: Fecha o arquivo .svg após a geração do conteúdo
    // if(fecharSvg(arqSvg) != 0){
    //     fprintf(stderr, "ERRO: Fechar o arquivo .svg após a geração do conteúdo.\n");
    //     return -1;
    // }
    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarGeo(Param* param, HashBin* h, Quadras* q){
    // Inicializa o buffer para o caminho completo do arquivo .geo
    char caminhoGeo[512];   

    // 1: Monta o caminho completo do arquivo .geo
    if(montarCaminhoGeo(param, caminhoGeo) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .geo.\n");
        return -1;
    }

    // 2: Abre o arquivo .geo para leitura
    FILE* arquivoGeo = fopen(caminhoGeo, "r");
    if(arquivoGeo == NULL){
        fprintf(stderr, "ERRO: Nao foi possivel abrir o arquivo .geo: %s\n", caminhoGeo);
        return -1;
    }

    // 3: Verifica se a estrutura de dados para armazenar os dados do arquivo .geo foi criada com sucesso
    if(q == NULL){
        fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .geo.\n");
        fclose(arquivoGeo);
        return -1;
    }

    // 4: Lê e processa os dados do arquivo .geo
    if(readFileGeo(arquivoGeo, h, q, param) != 0){
        fprintf(stderr, "ERRO: Leitura do arquivo .geo.\n");
        fclose(arquivoGeo);
        return -1;
    }

    // 5: Fecha o arquivo .geo após o processamento
    fclose(arquivoGeo);
    return 0;
}
/*###############################################################################################*/