#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"



/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct qry{
    // 1: Comando do arquivo .qry
    char*  comando;
    /** 1.1 - COMANDO: @o?
     * Armazena a posição geográfica do endereço cep/face/num no registrador reg.
     * SVG: linha vertical pontilhada vermelha mostrando a posição do endereço 
     * e colocar o número do registrador na outra extremidade do linha (topo da página)
     * TXT: reportar a coordenada relativa ao endereço
     */
    /** 1.2 - COMANDO: mvm
     * Atualiza a velocidade média das arestas dentro da região (x,y,w,h) para v.
     */
    /** 1.3 - COMANDO: regs
     * Considera como insuficiente os trechos com velocidade média inferior a vl. Calcula os componentes conexos.
     * SVG: Calcular os bounding boxes de cada componente conexo 
     * e desenhar os respectivos retângulos com cores diferentes e transparência de 50%.
     * TXT: reportar o número de componentes conexos.
     */
    /** 1.4 - COMANDO: exp
     * Calcula a árvore geradora mínima (seleciona apenas as arestas com velocidade média inferior a vl)
     * aumenta em 50% a velocidade média das arestas selecionadas.
     * SVG: pintar as arestas selecionadas com linha grossa e vermelha. 
     */
    /** 1.5 - COMANDO: p?
     * Determina o melhor trajeto entre a origem (reg1) e o destino (reg2). 
     * Desenhar os percursos (mais curto, mais rápido) com as cores cc e cr, respectivamente.
     * TXT: descrição textual do percurso. Caso destino inacessível, reportar.
     * SVG: mostrar os percursos pintados e animados. Marcar os extremos do percurso com “placas” com as letras I e F.
     */

    // 2: Parâmetros associados aos comandos
    char *reg, *cep, *face, *num;
    char *v, *x, *y, *w, *h;
    char *vl;
    char *reg1, *reg2, *cc, *cr;
}Qry;

typedef struct registrador{
    char id[16];
    double x;
    double y;
    int ativoFlag; // 0 = vazio | 1 = ocupado
} Registrador;

#define MAX_REGS 100
Registrador bancoRegistradores[MAX_REGS];

const char* CORES_ILHAS[] = {"red", "blue", "green", "orange", "purple", "cyan", "magenta", "yellow", "brown", "pink"};
const int NUM_CORES = 10;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoQry(Param* param, char* caminhoQry){
    // 1: Montar o caminho completo do arquivo .qry 
    // a partir do diretório de entrada e do nome do arquivo .qry 
    // fornecidos como argumentos na linha de comando, e armazenados na estrutura de parâmetros
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomeQry    = getNomeQry           (param);

    // 2: Imprime o nome do arquivo .qry original para depuração
    printf("Diretorio de entrada completo: \t\t%s\n", dirEntrada);
    printf("Arquivo .qry fornecido: \t\t%s\n", nomeQry);
    
    // 3: Concatena o diretório de entrada completo com o nome do arquivo .qry
    strcpy(caminhoQry, dirEntrada);
    strcat(caminhoQry, nomeQry);

    // 4: Imprime o caminho completo do arquivo .qry para depuração
    printf("Caminho completo do arquivo .qry: \t%s\n", caminhoQry);

    return 0;
}

int readFileQry(FILE* arquivoQry, HashBin* h, Param* param, Grafo* g){
    // 1: Prepara os arquivos de saída (SVG e TXT) para o processamento do arquivo .qry
    // 1.1: Busca diretório de saída
    char* dirSaida = getDirSaidaCompleto(param);    // Ex: "./saida/"

    // 1.2: Buffers para armazenar os nomes dos arquivos .geo e .qry sem os caminhos, 
    // para montar os nomes dos arquivos de saída corretamente
    char nomeGeoOriginal[256];  // Ex: "a.geo" (sem o caminho, apenas o nome do arquivo)
    char nomeQryOriginal[256];  // Ex: "b.qry" (sem o caminho, apenas o nome do arquivo)
    
    // 1.3: Busca os nomes dos arquivos .geo e .qry com os caminhos completos, 
    // para extrair apenas os nomes limpos (sem os caminhos) e montar os nomes dos arquivos de saída corretamente
    char *geoBruto = getNomeGeo(param); // Ex: "./entrada/a.geo" (com o caminho completo, como foi fornecido na linha de comando)
    char *qryBruto = getNomeQry(param); // Ex: "./entrada/b.qry" (com o caminho completo, como foi fornecido na linha de comando)

    // 1.4: Extrai apenas o nome do arquivo .geo e do arquivo .qry, ignorando o caminho/pastas que podem existir,
    char *apenasNomeGeo = strrchr(geoBruto, '/');   // Ex: "./entrada/a.geo" => "/a.geo"
    if(apenasNomeGeo != NULL) apenasNomeGeo++;      // Pula a barra '/' para obter apenas o nome do arquivo, sem o caminho. Ex: "/a.geo" => "a.geo"
    else apenasNomeGeo = geoBruto;                  // Se não tiver barra, já é o nome limpo. Ex: "a.geo" => "a.geo"
    char *apenasNomeQry = strrchr(qryBruto, '/');   // Ex: "./entrada/b.qry" => "/b.qry"
    if(apenasNomeQry != NULL) apenasNomeQry++;      // Pula a barra '/' para obter apenas o nome do arquivo, sem o caminho. Ex: "/b.qry" => "b.qry"
    else apenasNomeQry = qryBruto;                  // Se não tiver barra, já é o nome limpo. Ex: "b.qry" => "b.qry"

    // 1.5: Copia os nomes limpos para as variáveis locais
    strcpy(nomeGeoOriginal, apenasNomeGeo); // nomeGeoOriginal = "a.geo"
    strcpy(nomeQryOriginal, apenasNomeQry); // nomeQryOriginal = "b.qry"

    // 1.6: Remove extensões das cópias locais
    char* p;                            // Ponteiro auxiliar para encontrar a posição do ponto '.' que indica o início da extensão do arquivo
    p = strrchr(nomeGeoOriginal, '.');  // Ex: "a.geo" => ".geo"
    if(p) *p = '\0';                    // Substitui o ponto pela terminação da string para remover a extensão. Ex: "a.geo" => "a"
    p = strrchr(nomeQryOriginal, '.');  // Ex: "b.qry" => ".qry"
    if(p) *p = '\0';                    // Substitui o ponto pela terminação da string para remover a extensão. Ex: "b.qry" => "b"

    // 1.7: Monta Caminho do SVG BASE 
    // (O arquivo .svg gerado a partir do arquivo .geo, que servirá de base para o clone do SVG QRY)
    // dirSaida + nomeGeo + .svg
    // Ex: "./saida/" + "a" + ".svg" => "./saida/a.svg"
    char caminhoSvgBase[512];                   // Variável para armazenar o caminho completo do arquivo .svg
    strcpy(caminhoSvgBase, dirSaida);           // Ex: caminhoSvgBase = "./saida/"
    strcat(caminhoSvgBase, nomeGeoOriginal);    // Ex: "./saida/" + "a" => "./saida/a"
    strcat(caminhoSvgBase, ".svg");             // Ex: "./saida/a" + ".svg" => "./saida/a.svg"

    // 1.8: Montar Caminho do SVG QRY
    // (O arquivo .svg que será gerado a partir do arquivo .qry, clonando o SVG BASE e adicionando os elementos gráficos correspondentes aos comandos do arquivo .qry)
    // dirSaida + nomeGeo + "-" + nomeQry + .svg
    // Ex: "./saida/" + "a" + "-" + "b" + ".svg" => "./saida/a-b.svg"
    char caminhoSvgQry[512];                   // Variável para armazenar o caminho completo do arquivo .svg
    strcpy(caminhoSvgQry, dirSaida);           // Ex: caminhoSvgQry = "./saida/"
    strcat(caminhoSvgQry, nomeGeoOriginal);    // Ex: "./saida/" + "a" => "./saida/a"
    strcat(caminhoSvgQry, "-");                // Ex: "./saida/a" + "-" => "./saida/a-"
    strcat(caminhoSvgQry, nomeQryOriginal);    // Ex: "./saida/a-" + "b" => "./saida/a-b"
    strcat(caminhoSvgQry, ".svg");             // Ex: "./saida/a-b" + ".svg" => "./saida/a-b.svg"

    // 1.9: Montar Caminho do TXT (arquivo de saída para os resultados das consultas do arquivo .qry)
    // dirSaida + nomeGeo + "-" + nomeQry + .txt
    // Ex: "./saida/" + "a" + "-" + "b" + ".txt" => "./saida/a-b.txt"
    char caminhoTxtQry[512];                    // Variável para armazenar o caminho completo do arquivo .txt
    strcpy(caminhoTxtQry, dirSaida);            // Ex: caminhoTxtQry = "./saida/"
    strcat(caminhoTxtQry, nomeGeoOriginal);     // Ex: "./saida/" + "a" => "./saida/a"
    strcat(caminhoTxtQry, "-");                 // Ex: "./saida/a" + "-" => "./saida/a-"
    strcat(caminhoTxtQry, nomeQryOriginal);     // Ex: "./saida/a-" + "b" => "./saida/a-b"
    strcat(caminhoTxtQry, ".txt");              // Ex: "./saida/a-b" + ".txt" => "./saida/a-b.txt"

    // 1.10: Clona o SVG BASE para o SVG QRY
    FILE* qrySVG = clonarSvgBase(caminhoSvgBase, caminhoSvgQry);    
    if(qrySVG == NULL){
        printf("[ERROR]\n");
        printf("In qry.c [readFileQry();]: Failed to clone the base .svg file to the .qry .svg file.\n\n");
        return -1;
    }
    
    // 1.11: Abre o arquivo de saída .txt para escrita dos resultados das consultas do arquivo .qry
    FILE* qryTXT = fopen(caminhoTxtQry, "w");
    if(qryTXT == NULL){
        printf("[ERROR]\n");
        printf("In qry.c [readFileQry();]: Failed to create the .txt file for the .qry results: %s\n\n", caminhoTxtQry);
        return -1;
    }
    
    // 2: Lê o arquivo linha por linha
    // Inicializa o buffer para leitura das linhas do arquivo .qry 
    char linha[256];
    // Enquanto houver linhas para ler no arquivo .qry, processar cada linha
    while(fgets(linha, sizeof(linha), arquivoQry) != NULL){

        // 2.1: Limpeza da linha
        linha[strcspn(linha, "\n")] = '\0'; // Remove o ENTER do final da linha, se existir
        linha[strcspn(linha, "\r")] = '\0'; // Previne bugs de quebra de linha do Windows
        if(strlen(linha) == 0) continue;    // Ignora linhas em branco
        
        // Precedido de um [*] para facilitar a identificação das linhas do arquivo .qry no meio dos outros prints de depuração
        fprintf(qryTXT, "\n[*] %s\n", linha);

        // 2.2: Extrai apenas o primeiro token da linha, que corresponde ao comando do arquivo .qry, para identificar qual comando deve ser processado
        // Cria uma cópia da linha para usar na extração do comando, para não perder a linha original para os próximos comandos
        char *bufferLinha = strdup(linha);
        // Buffer para armazenar o comando extraído da linha do arquivo .qry
        char* comando = NULL;
        // Extrai o primeiro token da linha, usando espaço como delimitador. Ex: "mvm 50 10 10 100 100" => "mvm"
        comando = strtok(bufferLinha, " ");

        // 2.3: Processa o comando lido do arquivo .qry
        if(comando != NULL){
            /** COMANDO: @0? reg cep face num
             * Armazena a posição geográfica do endereço cep/face/num no registrador reg.
             */
            if(strcmp(comando, "@o?") == 0){
                // Inicializa buffers para os parâmetros do comando @o? (reg, cep, face e num)
                char reg[100]  = "";
                char cep[100]  = "";
                char face[100] = "";
                char num[100]  = "";

                // Extrai os parâmetros do comando @o? (reg, cep, face e num)
                sscanf(linha, "%*s %s %s %s %s", reg, cep, face, num);

                // Verifica se os parâmetros do comando @o? foram lidos corretamente
                if(reg[0] != '\0' && cep[0] != '\0' && face[0] != '\0' && num[0] != '\0'){
                    if(armazenarPosGeografica(h, reg, cep, face, num, qrySVG, qryTXT) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to store the geographic position of the address in the register.\n\n");
                        free(bufferLinha);
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameters for command @o? read from the .qry file.\n");
                    printf("[reg:\t%s]\n[cep:\t%s]\n[face:\t%s]\n[num:\t%s]\n\n", reg, cep, face, num);
                    free(bufferLinha);
                    return -1;
                }
            }

            /** COMANDO: mvm v x y w h
             * Atualiza a velocidade média das arestas dentro da região (x,y,w,h) para v.
             */
            if(strcmp(comando, "mvm") == 0){
                // Inicializa buffers para os parâmetros do comando mvm (v, x, y, w e h)
                char v[100] = "";
                char x[100] = "";
                char y[100] = "";
                char w[100] = "";
                char h[100] = "";
                
                // Extrai os parâmetros do comando mvm (v, x, y, w, h)
                sscanf(linha, "%*s %s %s %s %s %s", v, x, y, w, h);

                // Verifica se os parâmetros do comando mvm foram lidos corretamente
                if(v[0] != '\0' && x[0] != '\0' && y[0] != '\0' && w[0] != '\0' && h[0] != '\0'){
                    if(atualizarVelocidade(g, v, x, y, w, h) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to update the average speed of the edges.\n\n");
                        free(bufferLinha);
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameters for command mvm read from the .qry file.\n");
                    printf("[v:\t%s]\n[x:\t%s]\n[y:\t%s]\n[w:\t%s]\n[h:\t%s]\n\n", v, x, y, w, h);
                    free(bufferLinha);
                    return -1;
                }
            }

            /** COMANDO: regs vl
             * Considera como insuficiente os trechos com velocidade média inferior a vl. Calcula os componentes conexos.
             */
            if(strcmp(comando, "regs") == 0){
                // Inicializa buffer para o parâmetro do comando regs (vl)
                char vl[100] = "";

                // Extrai o parâmetro do comando regs (velocidade média limite)
                sscanf(linha, "%*s %s", vl);

                // Verifica se o parâmetro do comando regs foi lido corretamente
                if(vl[0] != '\0'){
                    if(calcularComponentesConexos(g, vl, qrySVG, qryTXT) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to calculate the connected components.\n\n");
                        free(bufferLinha);
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameter for command regs read from the .qry file.\n");
                    printf("[vl:\t%s]\n\n", vl);
                    free(bufferLinha);
                    return -1;
                }
            }

            /** COMANDO: exp vl
             * Calcula a árvore geradora mínima (seleciona apenas as arestas com velocidade média inferior a vl)
             * aumenta em 50% a velocidade média das arestas selecionadas
             */
            if(strcmp(comando, "exp") == 0){
                // Inicializa buffer para o parâmetro do comando exp (vl)
                char vl[100] = "";

                // Extrai o parâmetro do comando exp (velocidade média limite)
                sscanf(linha, "%*s %s", vl);

                // Verifica se o parâmetro do comando exp foi lido corretamente
                if(vl[0] != '\0'){
                    if(calcularArvoreGeradoraMinima(g, vl, qrySVG) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to calculate the minimum spanning tree.\n\n");
                        free(bufferLinha);
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameter for command exp read from the .qry file.\n");
                    printf("[vl:\t%s]\n\n", vl);
                    free(bufferLinha);
                    return -1;
                }
            }

            /** COMANDO: p? reg1 reg2 cc cr
             * Determina o melhor trajeto entre a origem (reg1) e o destino (reg2). 
             * Desenhar os percursos (mais curto, mais rápido) com as cores cc e cr, respectivamente.
             */
            if(strcmp(comando, "p?") == 0){
                // Inicializa buffers para os parâmetros do comando p? (reg1, reg2, cc e cr)
                char reg1[100] = "";
                char reg2[100] = "";
                char cc[100]   = "";
                char cr[100]   = "";
                
                // Extrai os parâmetros do comando p?
                sscanf(linha, "%*s %s %s %s %s", reg1, reg2, cc, cr);

                // Verifica se os parâmetros do comando p? foram lidos corretamente
                if(reg1[0] != '\0' && reg2[0] != '\0' && cc[0] != '\0' && cr[0] != '\0'){
                    if(calcularMelhorTrajeto(g, reg1, reg2, cc, cr, qrySVG, qryTXT) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to calculate the best route.\n\n");
                        free(bufferLinha);
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameters for command p? read from the .qry file.\n");
                    printf("[reg1:\t%s]\n[reg2:\t%s]\n[cc:\t%s]\n[cr:\t%s]\n\n", reg1, reg2, cc, cr);
                    free(bufferLinha);
                    return -1;
                }
            }
        }

        // 2.4: Se o comando lido do arquivo .qry for NULL, imprime uma mensagem de erro e continua para a próxima linha
        else{
            printf("[ERROR]\n");
            printf("In qry.c [readFileQry();]: Command read from the .qry file is NULL.\n");
            printf("[comando:\t%s]\n\n", comando);
            free(bufferLinha);
            return -1;
        }

        free(bufferLinha);
    }

    // 3: Fecha os arquivos de saída após o processamento do arquivo .qry
    fclose(qryTXT);
    if(fecharSvg(qrySVG) != 0){
        printf("[ERROR]\n");
        printf("In qry.c [readFileQry();]: Failed to close the .svg file.\n\n");
        return -1;
    }
    printf("Arquivo .svg fechado com sucesso apos a geracao do conteudo.\n");

    return 0;
}

FILE* clonarSvgBase(char* caminhoSvgBase, char* caminhoSvgQry){
    // 1: Abre o arquivo .svg base para leitura e o arquivo .svg do .qry para escrita 
    // (será criado a partir do clone do .svg base)
    FILE* arqBase = fopen(caminhoSvgBase, "r"); // Abre o arquivo .svg base para leitura
    if(arqBase == NULL){
        printf("[ERROR]\n");
        printf("In qry.c [clonarSvgBase();]: Failed to open the base .svg file.\n\n");
        return NULL;
    }
    FILE* arqQry = fopen(caminhoSvgQry, "w");   // Abre o arquivo .svg do .qry para escrita (será criado a partir do clone do .svg base)
    if(arqBase == NULL || arqQry == NULL){
        printf("[ERROR]\n");
        printf("In qry.c [clonarSvgBase();]: Failed to open the .svg files.\n\n");
        return NULL;
    }

    // 2: Lê o arquivo .svg base linha por linha e escreve no arquivo .svg do .qry, até encontrar a tag de fechamento </svg>
    char linha[1024];
    while(fgets(linha, sizeof(linha), arqBase)){
        // Se encontrar a tag de fechamento, NÃO copia ela. 
        // Vamos deixar o arquivo "aberto" para novos desenhos.
        // strstr() retorna um ponteiro para a primeira ocorrência da substring "</svg>" na linha, ou NULL se não for encontrada.
        if(strstr(linha, "</svg>") != NULL) continue;
        
        fputs(linha, arqQry);
    }

    // 3: Fecha o arquivo .svg base, mas deixa o arquivo .svg do .qry aberto para receber os desenhos do arquivo .qry
    fclose(arqBase);
    
    // 4: Retorna o ponteiro do novo arquivo pronto para receber os desenhos do .qry
    return arqQry; 
}

int calcularCoordenadaEndereco(Quadras* q, char face, int num, double* posX, double* posY){
    // 1: Obtém as coordenadas (x, y) e dimensões (w, h) da quadra
    double x = getQuadraX(q);
    double y = getQuadraY(q);
    double w = getQuadraW(q);
    double h = getQuadraH(q);

    /** 2: Calcula as coordenadas (x, y) do endereço com base na face e número, usando as coordenadas e dimensões da quadra
     * NORTE: O endereço está na parte superior da quadra,
     * então x varia com o número e y é constante (y da quadra)
     * 
     * SUL: O endereço está na parte inferior da quadra,
     * então x varia com o número e y é constante (y + altura da quadra)
     * 
     * LESTE: O endereço está na parte direita da quadra,
     * então x é constante (x + largura da quadra) e y varia com o número
     * 
     * OESTE: O endereço está na parte esquerda da quadra,
     * então x é constante (x da quadra) e y varia com o número
     */
    if      (face == 'N') { *posX = (x + num); *posY = (y)      ; return 0; }
    else if (face == 'S') { *posX = (x + num); *posY = (y + h)  ; return 0; }
    else if (face == 'L') { *posX = (x + w)  ; *posY = (y + num); return 0; }
    else if (face == 'O') { *posX = (x)      ; *posY = (y + num); return 0; }
    else{
        printf("[ERROR]\n");
        printf("In qry.c [calcularCoordenadaEndereco();]: Address face invalid. Must be one of the following: N, S, L, O.\n");
        printf("[face:\t%c]\n\n", face);
        return -1;
    }
}

char* getDirecaoCardeal(double x1, double y1, double x2, double y2){
    // 1: Calcula a diferença entre as coordenadas (x1, y1) e (x2, y2)
    double dx = x2 - x1;
    double dy = y2 - y1;

    // 2: Determina a direção cardeal com base na diferença das coordenadas
    // 2.1: Se a diferença em x for maior que a diferença em y, o movimento é horizontal
    if(abs((int)dx) > abs((int)dy)){
        // Se dx for positivo, o movimento é para o Leste; se negativo, para o Oeste
        if(dx > 0) return "Leste";
        else       return "Oeste";
    }
    // 2.2: Se a diferença em y for maior ou igual à diferença em x, o movimento é vertical    
    else{
        // Se dy for positivo, o movimento é para o Sul; se negativo, para o Norte
        if (dy > 0) return "Sul";
        else        return "Norte";
    }
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarQry(Param* param, HashBin* h, Grafo* g){
    // Inicializa o buffer para o caminho completo do arquivo .qry
    char caminhoQry[512];

    // 1: Monta o caminho completo do arquivo .qry
    if(montarCaminhoQry(param, caminhoQry) != 0){
        printf("[ERROR]\n");
        printf("In qry.c [processarQry();]: Failed to build the complete path for the .qry file.\n\n");
        return -1;
    }

    printf("Iniciando o processamento do arquivo .qry\n\n");

    // 2: Abre o arquivo .qry para leitura
    FILE* arquivoQry = fopen(caminhoQry, "r");
    if(arquivoQry == NULL){
        printf("[ERROR]\n");
        printf("In qry.c [processarQry();]: Failed to open the .qry file: %s\n\n", caminhoQry);
        return -1;
    }

    // 3: Lê e processa os dados do arquivo .qry
    if(readFileQry(arquivoQry, h, param, g) != 0){ 
        printf("[ERROR]\n");
        printf("In qry.c [processarQry();]: Failed to read the .qry file: %s\n\n", caminhoQry);
        fclose(arquivoQry);
        return -1;
    }

    // 4: Fecha o arquivo .qry após o processamento
    fclose(arquivoQry);
    printf("\nArquivo .qry processado com sucesso!\n");
    return 0;
}

Qry* criarQry(){
    // 1: Aloca memória para o objeto Qry
    Qry* qry = (Qry*)malloc(sizeof(Qry));
    if(qry == NULL){
        printf("[ERROR]\n");
        printf("In qry.c [criarQry();]: Memory allocation failed for Qry object.\n\n");
        return NULL;
    }

    // 2: Inicializa os campos do objeto Qry com valores padrão (NULL para strings, '\0' para char, 0 para int)
    qry->comando   = NULL;
    qry->reg  = NULL; qry->cep  = NULL; qry->face = NULL; qry->num = NULL;
    qry->v    = NULL; qry->x    = NULL; qry->y    = NULL; qry->w   = NULL; qry->h = NULL;
    qry->vl   = NULL;
    qry->reg1 = NULL; qry->reg2 = NULL; qry->cc   = NULL; qry->cr  = NULL;

    // 3: Retorna o ponteiro para o objeto Qry criado
    return qry;
}

int freeQry(Qry* qry){
    // 1: Verifica se o ponteiro do objeto Qry é NULL antes de tentar liberar a memória
    if(qry == NULL){
        printf("[ERROR]\n");
        printf("In qry.c [freeQry();]: Qry object is NULL.\n\n");
        return 0;
    }

    // 2: Libera a memória alocada para as strings dentro do objeto Qry, se não forem NULL
    free(qry->comando);
    free(qry->reg);  free(qry->cep);  free(qry->face); free(qry->num);
    free(qry->v);    free(qry->x);    free(qry->y);    free(qry->w);   free(qry->h);
    free(qry->vl);
    free(qry->reg1); free(qry->reg2); free(qry->cc);   free(qry->cr);

    // 3: Libera a memória alocada para o objeto Qry em si
    free(qry);
    return 0;
}

int armazenarPosGeografica(HashBin* h, char *reg, char *cep, char *face, char *num, FILE* qrySVG, FILE* qryTXT){
    // 1: Busca a quadra correspondente ao cep fornecido no hash binário
    Quadras* q = criarQuadra();
    if(buscarQuadra(h, cep, q) != 1){
        printf("[ERROR]\n");
        printf("In qry.c [armazenarPosGeografica();]: Failed to find the block with the given cep in the hash table.\n\n");
        freeQuadra(q);
        return -1;
    }

    // 2: Converte os parâmetros face e num para os tipos corretos (char e int)
    char f    = face[0];
    int n     = atoi(num);
    double px = 0.0, py = 0.0;

    // 3: Calcula as coordenadas (x, y) do endereço com base na quadra, face e número
    if(calcularCoordenadaEndereco(q, f, n, &px, &py) != 0){
        printf("[ERROR]\n");
        printf("In qry.c [armazenarPosGeografica();]: Failed to calculate the geographic coordinates of the address.\n\n");
        freeQuadra(q);
        return -1;
    }

    // 4: Armazena as coordenadas (x, y) no registrador identificado por reg
    // 4.1: Cria um flag para indicar se o registrador foi encontrado e atualizado
    int salvoFlag = 0;
    // 4.2: Percorre o banco de registradores para encontrar o registrador com o id correspondente a reg
    for(int i = 0; i < MAX_REGS; i++){
        // Se o registrador com o id correspondente a reg for encontrado 
        // ou se o registrador estiver vazio (ativoFlag == 0), 
        // atualiza as coordenadas (x, y) e marca o registrador como ocupado (ativoFlag = 1)
        if(strcmp(bancoRegistradores[i].id, reg) == 0 || bancoRegistradores[i].ativoFlag == 0){
            bancoRegistradores[i].x = px;
            bancoRegistradores[i].y = py;
            bancoRegistradores[i].ativoFlag = 1;
            salvoFlag = 1;  // Marca que o registrador foi encontrado e atualizado
            break;
        }
    }

    // 5: Se o registrador não foi encontrado e atualizado, imprime uma mensagem de erro
    if(!salvoFlag){
        printf("[ERROR]\n");
        printf("In qry.c [armazenarPosGeografica();]: Failed to store the geographic position in the register. No available register found.\n\n");
        freeQuadra(q);
        return -1;
    }

    // 6: TXT - Reporta a coordenada relativa ao endereço no arquivo .txt do .qry
    fprintf(qryTXT, "[INFO]: [@o?] %s %s %c %d\n", reg, cep, f, n);
    fprintf(qryTXT, "Address coordinates stored in register %s: (%.2lf, %.2lf)\n\n", reg, px, py);

    // 7: SVG
    // 7.1: Desenha a linha vertical pontilhada vermelha no arquivo .svg do .qry, mostrando a posição do endereço
    fprintf(qrySVG, "<line x1=\"%lf\" y1=\"0\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-dasharray=\"5,5\" stroke-width=\"2\" />\n", px, px, py);
    // 7.2: Coloca o número do registrador na outra extremidade da linha (topo da página)
    fprintf(qrySVG, "<text x=\"%lf\" y=\"15\" fill=\"red\" font-family=\"Arial\" font-size=\"14\">%s</text>\n", px + 5, reg);

    return 0;
}

int atualizarVelocidade(Grafo* g, char *v, char *x, char *y, char *w, char *h){
    // 1: Converte os parâmetros de string para double
    double nova_vm = atof(v);
    double rx = atof(x);
    double ry = atof(y);
    double rw = atof(w);
    double rh = atof(h);

    // 2: Atualiza a velocidade média das arestas dentro da região (x, y, w, h) para v
    atualizaVelocidadeRegiaoGrafo(g, rx, ry, rw, rh, nova_vm);

    return 0;
}

int calcularComponentesConexos(Grafo* g, char *vl_str, FILE* qrySVG, FILE* qryTXT){
    // 1: Converte o parâmetro de string para double
    double vl = atof(vl_str);

    // 2: Inicializa um ponteiro para armazenar as bounding boxes das ilhas de tráfego
    BoundingBox *bb = NULL;

    // 3: Calcula os componentes conexos
    int totalIlhas = identificarComponentesConexos(g, vl, &bb);

    // 4: TXT - Reporta a análise de vias com velocidade limite vl e o total de componentes conexos encontrados no arquivo .txt do .qry
    fprintf(qryTXT, "[INFO]: [regs] %s\n", vl_str);
    fprintf(qryTXT, "Arcs with speed limit of %.2lf analyzed\n", vl);
    fprintf(qryTXT, "[%d] Connected components found\n\n", totalIlhas);

    // 5: SVG - Desenha as bounding boxes das ilhas de tráfego no arquivo .svg do .qry
    // 5.1: Declara variáveis para armazenar as extremidades da bounding box
    double minX, minY, maxX, maxY;
    // 5.2: Percorre todas as bounding boxes das ilhas de tráfego e desenha um retângulo para cada uma delas no arquivo .svg
    for(int i = 0; i < totalIlhas; i++){
        // 5.2.1: Obtém as extremidades da bounding box da ilha de tráfego i
        if(getExtremidadesBB(bb, i, &minX, &minY, &maxX, &maxY) != 0){
            printf("[ERROR]\n");
            printf("In qry.c [calcularComponentesConexos();]: Failed to get the extremities of the bounding box for island %d.\n\n", i);
            return -1;
        }
        // 5.2.2: Calcula a largura e altura da bounding box
        double w = maxX - minX;
        double h = maxY - minY;

        // 5.2.3: Garante que a largura e altura mínima do retângulo seja 5.0 para melhor visualização no SVG
        if(w < 1.0) w = 5.0;
        if(h < 1.0) h = 5.0;

        // 5.2.4: Seleciona a cor da ilha de tráfego com base no índice i,
        // usando o array de cores CORES_ILHAS e o número total de cores NUM_CORES
        const char* corAtual = CORES_ILHAS[i % NUM_CORES];

        // 5.2.5: Desenha o retângulo da bounding box da ilha de tráfego i com a cor selecionada e opacidade de 0.5
        fprintf(qrySVG, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" "
                        "fill=\"%s\" fill-opacity=\"0.25\" stroke=\"%s\" stroke-width=\"2\" />\n",
                minX, minY, w, h, corAtual, corAtual);
    }

    // 6: Libera a memória alocada para as bounding boxes das ilhas de tráfego
    if(bb != NULL) {free(bb);}

    return 0;
}

int calcularArvoreGeradoraMinima(Grafo* g, char *vl_str, FILE* qrySVG){
    // 1: Converte o parâmetro de string para double
    double vl = atof(vl_str);

    // 2: Expande as vias com velocidade média inferior a vl
    ListaArestas* viasExpandidas = aumentaVMArestas(g, vl);
    if(viasExpandidas == NULL){
        printf("[ERROR]\n");
        printf("In qry.c [calcularArvoreGeradoraMinima();]: Failed to expand the edges with speed limit below %.2lf.\n\n", vl);
        return -1;
    }

    // 3: Obtém o total de vias expandidas
    int totalVias = getTamanhoListaArestas(viasExpandidas);
    if(totalVias == 0){
        printf("[INFO]\n");
        printf("In qry.c [calcularArvoreGeradoraMinima();]: No slow edges found with speed limit below %.2lf.\n\n", vl);
        destruirListaArestas(viasExpandidas);
        return 0;
    }

    // 4: Desenha as vias expandidas no SVG com destaque (linha mais grossa e vermelha)
    // 4.1: Declara variáveis para percorrer as vias expandidas e armazenar as coordenadas dos vértices de origem e destino
    double x1, y1, x2, y2;
    // 4.2: Percorre todas as vias expandidas e desenha cada uma delas no arquivo .svg do .qry
    for(int i = 0; i < totalVias; i++){
        // 4.2.1: Verifica se conseguiu obter as coordenadas dos vértices de origem e destino da via expandida i
        if(getCoordenadasArestaLista(viasExpandidas, i, &x1, &y1, &x2, &y2) == 0){
            // Desenha a via com destaque (linha mais grossa e colorida)
            fprintf(qrySVG, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"4\" />\n", x1, y1, x2, y2);
        }
    }

    // 5: Libera a memória alocada para a lista de vias expandidas
    destruirListaArestas(viasExpandidas);

    return 0;
}

int calcularMelhorTrajeto(Grafo* g, char *nomeReg1, char *nomeReg2, char *cc, char *cr, FILE* qrySVG, FILE* qryTXT){
    // 1: Converte os nomes dos registradores para índices inteiros
    int indiceR1 = atoi(&nomeReg1[1]);
    int indiceR2 = atoi(&nomeReg2[1]);

    // 2: Obtém as coordenadas (x, y) dos registradores de origem e destino
    double xOrigem  = bancoRegistradores[indiceR1].x;
    double yOrigem  = bancoRegistradores[indiceR1].y;
    double xDestino = bancoRegistradores[indiceR2].x;
    double yDestino = bancoRegistradores[indiceR2].y;

    // 3: Encontra os vértices mais próximos no grafo para as coordenadas de origem e destino
    int idVerticeOrigem  = DijkstraEncontrarVerticeMaisProximo(g, xOrigem, yOrigem);
    int idVerticeDestino = DijkstraEncontrarVerticeMaisProximo(g, xDestino, yDestino);

    // 4: Verifica se os vértices de origem e destino foram encontrados corretamente
    if(idVerticeOrigem == -1 || idVerticeDestino == -1){
        fprintf(qryTXT, "[ERROR]\n");
        fprintf(qryTXT, "In qry.c [calcularMelhorTrajeto();]: Failed to find the closest vertices for the given coordinates.\n");
        fprintf(qryTXT, "[Coordinates]: Origin (%.2lf, %.2lf), Destination (%.2lf, %.2lf)\n", xOrigem, yOrigem, xDestino, yDestino);
        fprintf(qryTXT, "[Vertex IDs]: Origin %d, Destination %d\n\n", idVerticeOrigem, idVerticeDestino);
        return -1;
    }

    // 5: Calcula os caminhos mais curto e mais rápido usando o algoritmo de Dijkstra
    Caminho* curto  = Dijkstra(g, idVerticeOrigem, idVerticeDestino, 'd');
    Caminho* rapido = Dijkstra(g, idVerticeOrigem, idVerticeDestino, 't');

    // 4: Desenhar no SVG e Escrever no TXT
    // 4.1: Menor caminho (mais curto)
    if(curto != NULL){
        // Obtém o número de passos do caminho mais curto
        int passosCurto = getTamanhoCaminho(curto);
        
        fprintf(qryTXT, "[INFO]: [p?] %s %s %s %s\n", nomeReg1, nomeReg2, cc, cr);
        fprintf(qryTXT, "TRAJETO MAIS CURTO:\n");

        // Percorre cada passo do caminho mais curto, exceto o último
        for(int i = 0; i < passosCurto - 1; i++){
            double x1, y1, x2, y2;
            char nomeRuaAtual[128]   = "";
            char nomeProximaRua[128] = "seu destino";

            getCoordenadasPasso(curto, i, &x1, &y1, &x2, &y2);
            strcpy(nomeRuaAtual, getNomeRuaPasso(curto, i));

            // Se não for o último passo, obtém o nome da próxima rua para a instrução no TXT
            if(i < passosCurto - 2) {strcpy(nomeProximaRua, getNomeRuaPasso(curto, i + 1));}

            fprintf(qrySVG, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"4\" />\n", 
                    x1, y1, x2, y2, cc);

            // 3. Escreve a instrução no TXT
            // Ex: "Siga na direção Leste na Rua Dom Bosco até o cruzamento com a Rua Maringa."
            char* direcao = getDirecaoCardeal(x1, y1, x2, y2);
            fprintf(qryTXT, "Passo %d: Siga na direcao %s na %s ate o cruzamento com a %s.\n", 
                    i + 1, direcao, nomeRuaAtual, nomeProximaRua);
        }
        fprintf(qryTXT, "Voce chegou ao seu destino!\n\n");
    }

    // 4.2: Caminho mais rápido (mais rápido)
    if(rapido != NULL){
        // Obtém o número de passos do caminho mais rápido
        int passosRapido = getTamanhoCaminho(rapido);
        
        fprintf(qryTXT, "[INFO]: [p?] %s %s %s %s\n", nomeReg1, nomeReg2, cc, cr);
        fprintf(qryTXT, "TRAJETO MAIS RAPIDO:\n");

        // Percorre cada passo do caminho mais rápido, exceto o último
        for(int i = 0; i < passosRapido - 1; i++){
            double x1, y1, x2, y2;
            char nomeRuaAtual[128]   = "";
            char nomeProximaRua[128] = "seu destino";

            getCoordenadasPasso(rapido, i, &x1, &y1, &x2, &y2);
            strcpy(nomeRuaAtual, getNomeRuaPasso(rapido, i));

            // Se não for o último passo, obtém o nome da próxima rua para a instrução no TXT
            if(i < passosRapido - 2) {strcpy(nomeProximaRua, getNomeRuaPasso(rapido, i + 1));}

            fprintf(qrySVG, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"4\" stroke-dasharray=\"5,5\" />\n", 
                    x1, y1, x2, y2, cr); // Usei dasharray (linha tracejada) para diferenciar visualmente da rota mais curta se elas se sobrepuserem!

            char* direcao = getDirecaoCardeal(x1, y1, x2, y2);
            fprintf(qryTXT, "Passo %d: Siga na direcao %s na %s ate o cruzamento com a %s.\n", 
                    i + 1, direcao, nomeRuaAtual, nomeProximaRua);
        }
        fprintf(qryTXT, "Voce chegou ao seu destino!\n\n");
    }

    // 5: Libera a memória alocada para os caminhos calculados
    destruirCaminho(curto);
    destruirCaminho(rapido);

    return 0;
}
/*###############################################################################################*/