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

int readFileQry(FILE* arquivoQry, HashBin* h, Param* param){
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
        fprintf(qryTXT, "[*] %s\n", linha);

        // 2.2: Extrai apenas o primeiro token da linha, que corresponde ao comando do arquivo .qry, para identificar qual comando deve ser processado
        // Cria uma cópia da linha para usar na extração do comando, para não perder a linha original para os próximos comandos
        char* bufferLinha = strdup(linha);
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
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameters for command @o? read from the .qry file.\n");
                    printf("[reg:\t%s]\n[cep:\t%s]\n[face:\t%s]\n[num:\t%s]\n\n", reg, cep, face, num);
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
                    if(atualizarVelocidade(v, x, y, w, h) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to update the average speed of the edges.\n\n");
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameters for command mvm read from the .qry file.\n");
                    printf("[v:\t%s]\n[x:\t%s]\n[y:\t%s]\n[w:\t%s]\n[h:\t%s]\n\n", v, x, y, w, h);
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
                    if(calcularComponentesConexos(vl, qrySVG, qryTXT) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to calculate the connected components.\n\n");
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameter for command regs read from the .qry file.\n");
                    printf("[vl:\t%s]\n\n", vl);
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
                    if(calcularArvoreGeradoraMinima(vl, qrySVG) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to calculate the minimum spanning tree.\n\n");
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameter for command exp read from the .qry file.\n");
                    printf("[vl:\t%s]\n\n", vl);
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
                    if(calcularMelhorTrajeto(reg1, reg2, cc, cr, qrySVG, qryTXT) != 0){
                        printf("[ERROR]\n");
                        printf("In qry.c [readFileQry();]: Failed to calculate the best route.\n\n");
                        return -1;
                    }
                }else{
                    printf("[ERROR]\n");
                    printf("In qry.c [readFileQry();]: Invalid parameters for command p? read from the .qry file.\n");
                    printf("[reg1:\t%s]\n[reg2:\t%s]\n[cc:\t%s]\n[cr:\t%s]\n\n", reg1, reg2, cc, cr);
                    return -1;
                }
            }
        }

        // 2.4: Se o comando lido do arquivo .qry for NULL, imprime uma mensagem de erro e continua para a próxima linha
        else{
            printf("[ERROR]\n");
            printf("In qry.c [readFileQry();]: Command read from the .qry file is NULL.\n");
            printf("[comando:\t%s]\n\n", comando);
            return -1;
        }
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
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarQry(Param* param, HashBin* h){
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
    if(readFileQry(arquivoQry, h, param) != 0){ 
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

int atualizarVelocidade(char *v, char *x, char *y, char *w, char *h){
    // 1: Implementar a lógica para atualizar a velocidade média das arestas dentro da região (x,y,w,h) para v
    // ...

    return 0;
}

int calcularComponentesConexos(char *vl, FILE* qrySVG, FILE* qryTXT){
    // 1: Implementar a lógica para considerar como insuficiente os trechos com velocidade média inferior a vl e calcular os componentes conexos
    // ...

    return 0;
}

int calcularArvoreGeradoraMinima(char *vl, FILE* qrySVG){
    // 1: Implementar a lógica para calcular a árvore geradora mínima (seleciona apenas as arestas com velocidade média inferior a vl) e aumentar em 50% a velocidade média das arestas selecionadas
    // ...

    return 0;
}

int calcularMelhorTrajeto(char *reg1, char *reg2, char *cc, char *cr, FILE* qrySVG, FILE* qryTXT){
    // 1: Implementar a lógica para determinar o melhor trajeto entre a origem (reg1) e o destino (reg2), desenhar os percursos (mais curto, mais rápido) com as cores cc e cr, respectivamente, e reportar a descrição textual do percurso no arquivo .txt do .qry
    // ...

    return 0;
}
/*###############################################################################################*/