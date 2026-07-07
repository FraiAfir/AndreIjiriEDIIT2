#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"

/*                             ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                          */
// Estrutura dos ATRIBUTOS das arestas (ruas/vias)
typedef struct infoAresta{
    char nome[64];          // Nome da rua/via
    char ldir[64];          // ID do vértice de origem
    char lesq[64];          // ID do vértice de destino
    double comprimento;     // Peso para o caminho mais curto
    double velocidadeMedia; // Peso para o caminho mais rápido
} InfoAresta;
// Estrutura do Arco (Aresta direcionada)
typedef struct arco{
    int vDestino;           // Índice do vértice vizinho no vetor (0, 1, 2, ... , nv-1) (nv => número máximo de vértices do grafo)
    InfoAresta* info;       // Atributos específicos da rua/via
    struct arco* proximo;   // Ponteiro para o próximo arco adjacente
} Arco;
// Estrutura do Vértice
typedef struct vertice{
    char id[64];    // Vetor de caracteres para o ID do vértice (ex: "v1", "v2", etc.)
    double x, y;    // Coordenadas espaciais do vértice
    int idIlha;     // Identificador da ilha à qual o vértice pertence (-1 = não pertence a nenhuma ilha)
    Arco* listaAdj; // Cabeça da lista encadeada de arestas que SAEM dele
} Vertice;
// Estrutura principal do Grafo
typedef struct grafo{
    int maxVertices;        // Capacidade total definida na criação (nv)
    int qntdAtual;          // Quantidade de vértices inseridos até o momento
    Vertice* vetorVertices; // Vetor dinâmico de Vértices [maxVertices]    
} Grafo;
typedef struct boundingbox{
    double minX, minY, maxX, maxY;
} BoundingBox;
/*################################################################################################*/





/*                                       FUNÇÕES AUXILIARES                                       */
int getNumVertices(Grafo* g){
    // 1: Verifica se o grafo é válido (não é NULL)
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [getNumVertices();]: Invalid Graph (NULL)\n");
        return -1;
    }

    // 2: Retorna a quantidade atual de vértices no grafo
    return g ? g->qntdAtual : 0;
}

int grafoAtualizarVelocidadeRegiao(Grafo* g, double x, double y, double w, double h, double novaVelocidade){
    // 1: Verifica se o grafo é válido (não é NULL)
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [grafoAtualizarVelocidadeRegiao();]: Invalid Graph (NULL)\n\n");
        return -1;
    }

    // 2: Determina as fronteiras da região retangular (x_min, x_max, y_min, y_max)
    double x_min = x;
    double x_max = x + w;
    double y_min = y;
    double y_max = y + h;

    // 3: Percorre todos os vértices do grafo para verificar se estão dentro da região retangular
    for (int i = 0; i < g->qntdAtual; i++){
        // 3.1: Obtém o vértice de origem
        Vertice orig = g->vetorVertices[i];

        // 3.2: Verifica se o vértice de origem está dentro da região retangular
        if(orig.x >= x_min && orig.x <= x_max && orig.y >= y_min && orig.y <= y_max){
            // 3.2.1: Obtém a cabeça da lista de adjacência do vértice de origem
            Arco* arcoAtual = orig.listaAdj;

            // 3.2.2: Percorre a lista de adjacência do vértice de origem para verificar os vértices de destino
            while(arcoAtual != NULL){
                // Obtém o vértice de destino correspondente ao arco atual
                Vertice dest = g->vetorVertices[arcoAtual->vDestino];

                // Verifica se o vértice de destino também está dentro da região retangular
                if(dest.x >= x_min && dest.x <= x_max && dest.y >= y_min && dest.y <= y_max){
                    // Atualiza a velocidade média da aresta (arco)
                    if(arcoAtual->info != NULL) {arcoAtual->info->velocidadeMedia = novaVelocidade;}
                }

                // Avança para o próximo arco na lista de adjacência do vértice de origem
                arcoAtual = arcoAtual->proximo;
            }
        }
    }

    return 0;
}

void DFSIlhas(Grafo* g, int indiceVertice, int idIlhaAtual, double vl, BoundingBox* bb){
    // 1: Marca o vértice atual como pertencente à ilha atual
    g->vetorVertices[indiceVertice].idIlha = idIlhaAtual;

    // 2: Atualiza o bounding box da ilha com as coordenadas do vértice atual
    double vx = g->vetorVertices[indiceVertice].x;
    double vy = g->vetorVertices[indiceVertice].y;

    // 3: Atualiza os limites do bounding box com base nas coordenadas do vértice atual
    if(vx < bb->minX) bb->minX = vx;
    if(vx > bb->maxX) bb->maxX = vx;
    if(vy < bb->minY) bb->minY = vy;
    if(vy > bb->maxY) bb->maxY = vy;

    // 4: Explora os vizinhos do vértice atual (arcos adjacentes) usando DFS
    // 4.1: Obtém a cabeça da lista de adjacência do vértice atual
    Arco* arco = g->vetorVertices[indiceVertice].listaAdj;
    // 4.2: Percorre a lista de adjacência do vértice atual enquanto houver arcos (arestas) disponíveis
    while(arco != NULL){
        // Verifica se a velocidade média da aresta é maior ou igual ao limite vl
        if(arco->info->velocidadeMedia >= vl){
            // Obtém o índice do vértice vizinho (destino) correspondente ao arco atual
            int vizinho = arco->vDestino;
            // Se o vizinho ainda não foi visitado, continua a exploração a partir dele através de recursão da DFS
            if(g->vetorVertices[vizinho].idIlha == -1) {DFSIlhas(g, vizinho, idIlhaAtual, vl, bb);}
        }

        // Avança para o próximo arco na lista de adjacência do vértice atual
        arco = arco->proximo; // Próxima rua
    }
}

int getExtremidadesBB(BoundingBox* bb, int indice, double* minX, double* minY, double* maxX, double* maxY){
    // 1: Verifica se o ponteiro do bounding box é válido (não é NULL)
    if(bb == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [getExtremidadesBB();]: Invalid Bounding Box (NULL)\n\n");
        return -1;
    }

    // 2: Atribui os valores das extremidades do bounding box aos ponteiros fornecidos
    *minX = bb[indice].minX;
    *minY = bb[indice].minY;
    *maxX = bb[indice].maxX;
    *maxY = bb[indice].maxY;

    return 0;
}
/*################################################################################################*/

/*                                         FUNÇÕES PRINCIPAIS                                     */
Grafo* criarGrafo(int maxVertices){
    // 1: Verifica se o número máximo de vértices é válido (maior que zero)
    if(maxVertices <= 0){
        printf("[ERROR]\n");
        printf("In grafo.c [criarGrafo();]: Invalid maximum number of vertices (%d)\n", maxVertices);
        return NULL;
    }

    // 2: Cria a estrutura do grafo e aloca memória para ela
    // 2.1: Aloca memória para a estrutura do grafo
    Grafo* g = (Grafo*)malloc(sizeof(struct grafo));
    // 2.2: Verifica se a alocação de memória foi bem-sucedida
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [criarGrafo();]: Failed to allocate memory for the graph\n");
        return NULL;
    }
    // 2.3: Inicializa os atributos do grafo
    g->maxVertices = maxVertices;
    g->qntdAtual   = 0;
    
    // 3: Cria o vetor de vértices do grafo e aloca memória para ele
    // 3.1: Aloca o vetor de vértices de forma limpa
    g->vetorVertices = (Vertice*)calloc(maxVertices, sizeof(Vertice));
    // 3.2: Verifica se a alocação de memória para o vetor de vértices foi bem-sucedida
    if(g->vetorVertices == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [criarGrafo();]: Failed to allocate memory for the vertices array\n");
        free(g);
        return NULL;
    }
    // 3.3: Inicializa os atributos de cada vértice no vetor de vértices (ID, coordenadas, lista de adjacência)
    for(int i = 0; i < maxVertices; i++){
        g->vetorVertices[i].id[0] = '\0';
        g->vetorVertices[i].x = 0.0;
        g->vetorVertices[i].y = 0.0;
        g->vetorVertices[i].listaAdj = NULL;
    }

    return g;
}

int freeGrafo(Grafo* g){
    // 1: Verifica se o grafo é válido (não é NULL)
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [freeGrafo();]: Invalid Graph (NULL)\n");
        return -1;
    }

    // 2: Percorre os vértices inseridos no grafo (de 0 até qntdAtual - 1) para liberar a memória das listas de adjacência
    for(int i = 0; i < g->qntdAtual; i++){
        Arco* atual = g->vetorVertices[i].listaAdj;
        // 2.1: Percorre a lista de adjacência do vértice atual e libera a memória de cada arco e seus atributos
        while(atual != NULL){
            Arco* prox = atual->proximo;
            if(atual->info != NULL) {free(atual->info);}    // Libera a memória dos atributos da aresta (InfoAresta)
            free(atual);                                    // Libera a memória do arco atual
            atual = prox;                                   // Avança para o próximo arco na lista
        }
    }

    // 3: Libera a memória do vetor de vértices do grafo
    if(g->vetorVertices != NULL) {free(g->vetorVertices);}

    // 4: Libera a estrutura principal do grafo
    free(g);

    return 0;
}

int inserirVertice(Grafo* g, char* id, double x, double y){
    // 1: Verifica se o grafo é válido (não é NULL) e se ainda há capacidade para inserir um novo vértice
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [inserirVertice();]: Invalid Graph (NULL)\n");
        return -1;
    }
    if(g->qntdAtual >= g->maxVertices){
        printf("[ERROR]\n");
        printf("In grafo.c [inserirVertice();]: Maximum number of vertices reached (%d)\n", g->maxVertices);
        return -1;
    }

    // 2: Obtém o índice inteiro para o novo vértice usando a quantidade atual de vértices (qntdAtual) como base para o próximo índice disponível
    int idx = g->qntdAtual; // O próximo índice disponível é igual à quantidade atual de vértices, pois os índices começam em 0
    
    // 3: Preenche os dados do vértice no vetor de vértices do grafo usando o índice obtido
    strcpy(g->vetorVertices[idx].id, id);   // Copia o ID do vértice para a estrutura do vértice
    g->vetorVertices[idx].x = x;            // Atribui a coordenada x do vértice
    g->vetorVertices[idx].y = y;            // Atribui a coordenada y do vértice
    g->vetorVertices[idx].listaAdj = NULL;  // Inicializa a lista de adjacência do vértice como NULL (sem arestas inicialmente)

    // 4: Atualiza a quantidade atual de vértices no grafo (qntdAtual) para refletir a inserção do novo vértice
    g->qntdAtual++;

    // 5: Retorna 0 para indicar sucesso na inserção do vértice
    return 0;
}

int inserirAresta(Grafo* g, int i, int j, char* ldir, char* lesq, double cmp, double vm, char* nomeRua){
    // 1: Verifica se o grafo é válido (não é NULL)
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [inserirAresta();]: Invalid Graph (NULL)\n");
        return -1;
    }
    // 2: Verifica se os parâmetros de entrada são válidos (IDs dos vértices, nome da rua, etc.)
    if(i == -1 || j == -1 || ldir == NULL || lesq == NULL || nomeRua == NULL || cmp < 0 || vm < 0){
        printf("[ERROR]\n");
        printf("In grafo.c [inserirAresta();]: Invalid input parameters (NULL or non-positive values)\n");
        printf("[i:\t\t%d]\n[j:\t\t%d]\n[ldir:\t\t%p]\n[lesq:\t\t%p]\n[nomeRua:\t%s]\n[cmp:\t\t%f]\n[vm:\t\t%f]\n\n", 
            i, j, ldir, lesq, nomeRua, cmp, vm);
        return -1;
    }

    // 3: Cria e preenche a estrutura de atributos da aresta (rua/via)
    InfoAresta* info = (InfoAresta*)malloc(sizeof(InfoAresta));
    // 3.1: Verifica se a alocação de memória para os atributos da aresta foi bem-sucedida
    if(info == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [inserirAresta();]: Failed to allocate memory for the edge attributes\n");
        return -1;
    }
    // 3.2: Preenche os atributos da aresta (rua/via) usando os parâmetros de entrada fornecidos para a função, copiando os valores para a estrutura de atributos da aresta (InfoAresta)
    strcpy(info->nome, nomeRua);    // Copia o nome da rua para os atributos da aresta
    strcpy(info->ldir, ldir);       // Copia o ID do vértice de origem para os atributos da aresta
    strcpy(info->lesq, lesq);       // Copia o ID do vértice de destino para os atributos da aresta
    info->comprimento     = cmp;    // Atribui o comprimento da rua aos atributos da aresta
    info->velocidadeMedia = vm;     // Atribui a velocidade média da rua aos atributos da aresta

    // 4: Cria uma nova estrutura de arco (aresta direcionada) para representar a aresta que conecta o vértice de origem ao vértice de destino
    Arco* novoArco = (Arco*)malloc(sizeof(Arco));
    // 4.1: Verifica se a alocação de memória para o novo arco foi bem-sucedida
    if(novoArco == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [inserirAresta();]: Failed to allocate memory for the new edge\n");
        free(info); // Libera a memória alocada para os atributos da aresta antes de retornar
        return -1;
    }
    // 4.2: Preenche os dados do novo arco, atribuindo o índice do vértice de destino e os atributos da aresta (rua/via) à estrutura do arco
    novoArco->vDestino = j;     // Atribui o índice do vértice de destino ao arco
    novoArco->info     = info;  // Atribui os atributos da aresta (rua/via) ao arco  
    
    /** 5: Insere o novo arco na lista de adjacência do vértice de origem
     * Atuaiza os ponteiros para manter a estrutura da lista encadeada de arestas adjacentes
     * 
     * O próximo arco do novo arco aponta para o início da lista de adjacência atual do vértice de origem
     * Ex: novoArco|próx -> listaAdj[i]
     * 
     * O início da lista de adjacência do vértice de origem agora aponta para o novo arco, inserindo-o no início da lista
     * Ex: listaAdj[i] -> novoArco
     */
    novoArco->proximo = g->vetorVertices[i].listaAdj;
    g->vetorVertices[i].listaAdj = novoArco;

    // 6: Retorna 0 para indicar sucesso na inserção da aresta
    return 0;
}

int identificarComponentesConexos(Grafo* g, double vl, BoundingBox** vetorBB){
    // 1: Verifica se o grafo é válido (não é NULL)
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [identificarComponentesConexos();]: Invalid Graph (NULL)\n");
        return -1;
    }

    // 2: Inicializa todos os vértices como não pertencentes a nenhuma ilha (idIlha = -1)
    for(int i = 0; i < g->qntdAtual; i++) {g->vetorVertices[i].idIlha = -1;}

    // 3: Aloca memória para o vetor de Bounding Boxes (uma para cada ilha) com tamanho máximo igual à quantidade atual de vértices do grafo
    *vetorBB = (BoundingBox*)malloc(g->qntdAtual * sizeof(BoundingBox));
    // 3.1: Verifica se a alocação de memória para o vetor de Bounding Boxes foi bem-sucedida
    if(*vetorBB == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [identificarComponentesConexos();]: Failed to allocate memory for the bounding boxes array\n");
        return -1;
    }

    // 4: Identifica os componentes conexos do grafo, considerando apenas as arestas com velocidade média maior ou igual a vl
    // 4.1: Inicializa o contador de ilhas (componentes conexos) como zero
    int contadorIlhas = 0;
    // 4.2: Percorre todos os vértices do grafo para identificar os componentes conexos
    for(int i = 0; i < g->qntdAtual; i++){
        // 4.2.1: Verifica se o vértice atual ainda não pertence a nenhuma ilha (idIlha == -1)
        if(g->vetorVertices[i].idIlha == -1){
            // Iniciando um novo componente conexo a partir do vértice atual com valores iniciais extremos para o bounding box
            (*vetorBB)[contadorIlhas].minX = 999999.0;
            (*vetorBB)[contadorIlhas].minY = 999999.0;
            (*vetorBB)[contadorIlhas].maxX = -999999.0;
            (*vetorBB)[contadorIlhas].maxY = -999999.0;

            // Chama a função DFS para explorar todos os vértices conectados ao vértice atual,
            // marcando-os como pertencentes à ilha atual e atualizando o bounding box da ilha
            DFSIlhas(g, i, contadorIlhas, vl, &((*vetorBB)[contadorIlhas]));
            
            // Incrementa o contador de ilhas após a conclusão da exploração do componente conexo atual
            contadorIlhas++;
        }
    }

    return contadorIlhas;
}
/*################################################################################################*/