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



// Estrutura do Bounding Box
typedef struct boundingbox{
    double minX, minY, maxX, maxY;
} BoundingBox;



// Estrutura para armazenar as arestas selecionadas pelo algoritmo de Kruskal
typedef struct arestas_kruskal{
    int vOrigem;
    int vDestino;
    double comprimento;
    InfoAresta* info;
} ArestaKruskal;
// Estrutura para armazenar a lista de arestas resultante do algoritmo de Kruskal
typedef struct lista_arestas{
    ArestaKruskal* vetorArestas; // Vetor de arestas da AGM resultante do algoritmo de Kruskal
    int quantidade;              // Quantidade de arestas na AGM resultante
    Grafo* grafoOriginal;        // Ponteiro para o grafo original (para referência às coordenadas dos vértices)
} ListaArestas;
/*################################################################################################*/



/*                                       KRUSKAL | Union-Find                                     */
int compararArestasKruskal(void* a, void* b){
    // 1: Converte os ponteiros genéricos para ponteiros do tipo ArestaKruskal
    ArestaKruskal *a1 = (ArestaKruskal*)a;
    ArestaKruskal *a2 = (ArestaKruskal*)b;

    /** 2: Compara os comprimentos das arestas para determinar a ordem
     * Se a aresta a1 for menor que a2, retorna -1 (a1 vem antes de a2)
     * Se a aresta a1 for maior que a2, retorna 1 (a1 vem depois de a2)
     * Se forem iguais, retorna 0 (não há diferença na ordem)
     */
    if(a1->comprimento < a2->comprimento) return -1;
    if(a1->comprimento > a2->comprimento) return 1;
    return 0;
}
int uf_find(int i, int* pai){
    // 1: Verifica se o índice do vértice é o próprio pai (raiz do conjunto) e retorna o índice do pai correspondente
    if(pai[i] == i) return i;

    // 2: Se não for o próprio pai, realiza a busca recursiva para encontrar a raiz do conjunto e aplica compressão de caminho
    return pai[i] = uf_find(pai[i], pai);
}
void uf_union(int i, int j, int* pai){
    // 1: Encontra as raízes dos conjuntos aos quais os vértices i e j pertencem
    int raizI = uf_find(i, pai);
    int raizJ = uf_find(j, pai);

    // 2: Se as raízes forem diferentes, une os conjuntos fazendo a raiz de i apontar para a raiz de j
    if(raizI != raizJ) {pai[raizI] = raizJ;}
}
/*################################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                       */
int getCoordenadasVertice(Grafo* g, int indice, double* x, double* y){
    // 1: Verifica se o grafo é válido ou se o índice do vértice é válido (dentro do intervalo permitido)
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [getCoordenadasVertice();]: Invalid Graph\n\n");
        return -1;
    }
    if(indice < 0 || indice >= g->qntdAtual){
        printf("[ERROR]\n");
        printf("In grafo.c [getCoordenadasVertice();]: Invalid vertex index\n");
        printf("[indice:\t%d]\n[qntdAtual:\t%d]\n\n", indice, g->qntdAtual);
        return -1;
    }

    // 2: Obtém as coordenadas do vértice
    *x = g->vetorVertices[indice].x;
    *y = g->vetorVertices[indice].y;

    return 0;
}

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

int atualizaVelocidadeRegiaoGrafo(Grafo* g, double x, double y, double w, double h, double novaVelocidade){
    // 1: Verifica se o grafo é válido (não é NULL)
    if(g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [atualizaVelocidadeRegiaoGrafo();]: Invalid Graph (NULL)\n\n");
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

int getTamanhoListaArestas(ListaArestas* lista){
    // 1: Verifica se a lista de arestas é válida (não é NULL)
    if(lista == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [getTamanhoListaArestas();]: Invalid List of Edges (NULL)\n\n");
        return 0;
    }

    // 2: Retorna a quantidade de arestas na lista
    return lista->quantidade;
}

int getCoordenadasArestaLista(ListaArestas* lista, int indice, double* x1, double* y1, double* x2, double* y2){
    // 1: Verifica se a lista de arestas é válida (não é NULL) e se o índice fornecido está dentro do intervalo permitido
    if(lista == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [getCoordenadasArestaLista();]: Invalid List of Edges (NULL)\n\n");
        return -1;
    }
    if(indice < 0 || indice >= lista->quantidade){
        printf("[ERROR]\n");
        printf("In grafo.c [getCoordenadasArestaLista();]: Invalid edge index (%d)\n", indice);
        printf("[indice:\t%d]\n[quantidade:\t%d]\n\n", indice, lista->quantidade);
        return -1;
    }

    // 2: Obtém os índices dos vértices de origem e destino da aresta no vetor de arestas
    int idOrigem  = lista->vetorArestas[indice].vOrigem;
    int idDestino = lista->vetorArestas[indice].vDestino;

    // 3: Obtém as coordenadas dos vértices de origem e destino da aresta usando os índices obtidos
    *x1 = lista->grafoOriginal->vetorVertices[idOrigem].x;
    *y1 = lista->grafoOriginal->vetorVertices[idOrigem].y;
    *x2 = lista->grafoOriginal->vetorVertices[idDestino].x;
    *y2 = lista->grafoOriginal->vetorVertices[idDestino].y;

    return 0;
}

int destruirListaArestas(ListaArestas* lista){
    // 1: Verifica se a lista de arestas é válida (não é NULL)
    if(lista == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [destruirListaArestas();]: Invalid List of Edges (NULL)\n\n");
        return -1;
    }

    // 2: Verifica se o vetor de arestas é válido (não é NULL) e libera a memória alocada para ele
    if(lista->vetorArestas != NULL) free(lista->vetorArestas);

    // 3: Libera a memória alocada para a estrutura da lista de arestas
    free(lista);
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

ListaArestas* aumentaVMArestas(Grafo* g, double vl){
    // 1: Verifica se o grafo é válido (não é NULL)
    if (g == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [aumentaVMArestas();]: Invalid Graph (NULL)\n\n");
        return NULL;
    }

    // 2: Aloca memória para armazenar todas as arestas lentas (com velocidade < vl) do grafo
    int maxArestas = g->qntdAtual * 10;
    ArestaKruskal* todasArestas = (ArestaKruskal*)malloc(maxArestas * sizeof(ArestaKruskal));
    if(todasArestas == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [aumentaVMArestas();]: Failed to allocate memory for the edges array\n\n");
        return NULL;
    }

    // 3: Obtém as arestas do grafo com velocidade média menor que vl e armazena-as no vetor todasArestas
    // 3.1: Flag para contar o número total de arestas lentas encontradas
    int totalLentas = 0;
    // 3.2: Percorre todos os vértices do grafo para acessar suas listas de adjacência e identificar as arestas lentas
    for(int i = 0; i < g->qntdAtual; i++){
        // 3.2.1: Obtém a cabeça da lista de adjacência do vértice atual
        Arco* arco = g->vetorVertices[i].listaAdj;

        // 3.2.2: Percorre a lista de adjacência do vértice atual enquanto houver arcos (arestas) disponíveis
        while(arco != NULL){
            // Verifica se a velocidade média da aresta é menor que o limite vl
            if(arco->info->velocidadeMedia < vl){
                // Armazena a aresta lenta no vetor todasArestas, preenchendo os campos correspondentes
                todasArestas[totalLentas].vOrigem = i;
                todasArestas[totalLentas].vDestino = arco->vDestino;
                todasArestas[totalLentas].comprimento = arco->info->comprimento;
                todasArestas[totalLentas].info = arco->info;

                // Incrementa o contador de arestas lentas encontradas
                totalLentas++;
            }

            // Avança para o próximo arco na lista de adjacência do vértice atual
            arco = arco->proximo;
        }
    }

    // 4: Verifica se foram encontradas arestas lentas; caso contrário, libera a memória alocada e retorna NULL
    if(totalLentas == 0){
        printf("[INFO]\n");
        printf("In grafo.c [aumentaVMArestas();]: No edges with speed less than %.2f found in the graph\n\n", vl);
        free(todasArestas);
        return NULL;
    }

    /** 5: Ordena o vetor de arestas lentas pelo comprimento
     * qsort(void *_Base, size_t _NumOfElements, size_t _SizeOfElements, _CoreCrtNonSecureSearchSortCompareFunction _CompareFunction):
     * - void *_Base:                                                   Ponteiro para o primeiro elemento do vetor a ser ordenado
     * - size_t _NumOfElements:                                         Número de elementos no vetor a ser ordenado
     * - size_t _SizeOfElements:                                        Tamanho em bytes de cada elemento do vetor
     * - _CoreCrtNonSecureSearchSortCompareFunction _CompareFunction:   Ponteiro para a função de comparação usada para determinar a ordem dos elementos
     * 
     * qsort(todasArestas, totalLentas, sizeof(ArestaKruskal), compararArestasKruskal):
     * - todasArestas:              Ponteiro para o primeiro elemento do vetor de arestas lentas a ser ordenado
     * - totalLentas:               Número de arestas lentas no vetor a ser ordenado
     * - sizeof(ArestaKruskal):     Tamanho em bytes de cada elemento do vetor (tamanho da estrutura ArestaKruskal)
     * - compararArestasKruskal:    Ponteiro para a função de comparação usada para determinar a ordem das arestas com base no comprimento
     */
    qsort(todasArestas, totalLentas, sizeof(ArestaKruskal), compararArestasKruskal);

    // 6: Inicializa o vetor de pais para a estrutura de união-find
    // e define cada vértice como seu próprio pai inicialmente
    int* pai = (int*)malloc(g->qntdAtual * sizeof(int));
    for(int i = 0; i < g->qntdAtual; i++) pai[i] = i;

    // 7: Aloca memória para armazenar o resultado da AGM usando o algoritmo de Kruskal
    ArestaKruskal* agmKruskal = (ArestaKruskal*)malloc((g->qntdAtual + 1) * sizeof(ArestaKruskal));
    if(!agmKruskal == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [aumentaVMArestas();]: Failed to allocate memory for the MST result array\n\n");
        free(todasArestas);
        free(pai);
        return NULL;
    }

    // 8: Kruskal
    // 8.1: Inicializa o contador de arestas incluídas na AGM
    int contAGM = 0;
    // 8.2: Percorre todas as arestas lentas ordenadas pelo comprimento para construir a AGM
    for(int i = 0; i < totalLentas; i++){
        // 8.2.1: Obtém os índices dos vértices de origem e destino da aresta atual
        int u = todasArestas[i].vOrigem;
        int v = todasArestas[i].vDestino;

        // 8.2.2: Verifica se os vértices u e v pertencem a conjuntos diferentes
        if(uf_find(u, pai) != uf_find(v, pai)){
            // Inclui a aresta na AGM
            agmKruskal[contAGM] = todasArestas[i];
            // Aumenta a velocidade média da aresta incluída na AGM em 50%
            agmKruskal[contAGM].info->velocidadeMedia *= 1.5;
            // Incrementa o contador de arestas incluídas na AGM
            contAGM++;
            // Une os conjuntos dos vértices u e v na estrutura de união-find
            uf_union(u, v, pai);
        }
    }

    // 9: Finaliza o vetor de resultado da AGM com um elemento nulo para indicar o fim da lista
    agmKruskal[contAGM].info = NULL;

    // 10: Cria a estrutura ListaArestas para armazenar o resultado da AGM e retorna-a
    // 10.1: Aloca memória para a estrutura ListaArestas
    ListaArestas* resultado  = (ListaArestas*)malloc(sizeof(ListaArestas));
    // 10.2: Verifica se a alocação de memória para a estrutura ListaArestas foi bem-sucedida
    if(resultado == NULL){
        printf("[ERROR]\n");
        printf("In grafo.c [aumentaVMArestas();]: Failed to allocate memory for the MST result structure\n\n");
        free(todasArestas);
        free(pai);
        free(agmKruskal);
        return NULL;
    }
    // 10.3: Preenche os campos da estrutura ListaArestas com o vetor de arestas da AGM,
    // a quantidade de arestas incluídas e o ponteiro para o grafo original
    resultado->vetorArestas  = agmKruskal;
    resultado->quantidade    = contAGM;
    resultado->grafoOriginal = g;

    // 11: Libera a memória alocada para o vetor de todas as arestas lentas 
    // e para o vetor de pais da estrutura de união-find
    free(todasArestas);
    free(pai);

    // 12: Retorna a estrutura ListaArestas contendo o resultado da AGM obtida pelo algoritmo de Kruskal
    return resultado;
}
/*################################################################################################*/