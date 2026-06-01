#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"

/*                                       FUNÇÕES PRINCIPAIS                                      */
FILE* criarSvg(char* caminhoSvg, double largura, double altura){
    // 1: Abre o arquivo .svg para escrita
    FILE* arqSvg = fopen(caminhoSvg, "w");
    if(arqSvg == NULL){ 
        fprintf(stderr, "ERRO: Nao foi possivel criar o arquivo .svg\n");
        return NULL;
    }

    // 2: Escreve a declaração do elemento <svg> no arquivo .svg
    fprintf(arqSvg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%lf\" height=\"%lf\">\n", largura+100.0, altura+100.0);

    // 3: Retorna o ponteiro para o arquivo .svg criado
    return arqSvg;
}

int desenharFormaSvg(FILE* arqSvg, char* tipoForma, double x, double y, double w, double h, char* sw, char* cstrk, char* cfill){
    // 1: Verifica se o ponteiro para o arquivo .svg é nulo antes de tentar escrever nele
    if(arqSvg == NULL) return -1;

    // 2: Verifica o tipo da forma a ser desenhada e escreve a tag correspondente no arquivo .svg
    // 2.1: Para um retângulo, escreve a tag <rect> com os atributos x, y, width, height, stroke, stroke-width e fill
    if(strcmp(tipoForma, "r") == 0){
        fprintf(arqSvg, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\" />\n",
            x, y, w, h, cstrk, sw, cfill);
    }

    // 2.2: Para um círculo, escreve a tag <circle> com os atributos cx, cy, r, stroke, stroke-width e fill
    else if(strcmp(tipoForma, "c") == 0){
        // Calcula o raio do círculo com base na largura (w)
        double raio = w / 2.0;

        fprintf(arqSvg, "\t<circle fill=\"%s\" fill-opacity=\"0.5\" stroke=\"%s\" r=\"%lf\" cy=\"%lf\" cx=\"%lf\" />\n", 
            cfill, cstrk, raio, y + raio, x + raio);
    }

    // 2.3: Para um texto, escreve a tag <text> com os atributos x, y, font-size e fill
    else if(strcmp(tipoForma, "l") == 0){
        fprintf(arqSvg, "<text style=\"font-size:5px;fill:%s\" font-size=\"5\" y=\"%lf\" x=\"%lf\"> Text here />\n",
            cfill, y, x);
    }

    // 2.4: Em caso de um tipo de forma desconhecido, exibe uma mensagem de erro
    else fprintf(stderr, "ERRO: Tipo de forma desconhecido: %s\n", tipoForma);
    
    return 0;
}

int fecharSvg(FILE* arqSvg){
    // 1: Verifica se o ponteiro para o arquivo .svg é nulo antes de tentar escrever nele
    if(arqSvg == NULL) return -1;

    // 2: Escreve a tag de fechamento do elemento <svg> no arquivo .svg
    fprintf(arqSvg, "</svg>\n");

    // 3: Fecha o arquivo .svg
    fclose(arqSvg);

    return 0;
}
/*###############################################################################################*/