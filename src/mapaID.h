#ifndef _MAPA_ID_H
#define _MAPA_ID_H



typedef struct mapa Mapa;



/** DEFINIÇÃO DO MÓDULO
 * Este módulo de Mapa é responsável por criar e gerenciar um mapa que associa identificadores de vértices (IDs) 
 * a índices inteiros correspondentes em um vetor de vértices.
 * Utilizado principalmente para facilitar a busca e o acesso aos vértices do grafo, 
 * permitindo que operações como inserção de arestas e busca por caminhos entre vértices sejam realizadas de forma eficiente.
 */



/**
 * Esta função é responsável por alocar memória e inicializar uma estrutura de dados do tipo Mapa,
 * que será utilizada para armazenar a relação entre identificadores de vértices (IDs) 
 * e índices inteiros correspondentes em um vetor de vértices.
 * 
 * @param capacidade Capacidade inicial do mapa, geralmente baseada na quantidade de vértices (nv) do grafo
 * @return           Ponteiro para a estrutura de dados Mapa criada. NULL em caso de falha na alocação de memória
 */
Mapa* criarMapa(int capacidade);

/**
 * Esta função é responsável por inserir uma nova relação no mapa, associando um identificador de vértice (ID)
 * a um índice inteiro correspondente em um vetor de vértices.
 * 
 * Ex: ID string -> Índice inteiro | "v1" -> 0
 * 
 * @param mapa        Ponteiro para a estrutura de dados Mapa
 * @param id          Identificador de vértice (string)
 * @param indiceVetor Índice inteiro correspondente em um vetor de vértices
 * @return            0 se a inserção foi bem-sucedida. -1 caso contrário
 */
int inserirMapa(Mapa* mapa, char* id, int indiceVetor);

/**
 * Esta função é responsável por buscar um identificador de vértice (ID) no mapa 
 * e retornar o índice inteiro correspondente em um vetor de vértices. 
 * Caso o ID não seja encontrado, a função retorna -1.
 * 
 * @param mapa Ponteiro para a estrutura de dados Mapa
 * @param id   Identificador de vértice (string) a ser buscado
 * @return     Índice inteiro correspondente ao ID no vetor de vértices. -1 se não encontrado
 */
int buscarMapa(Mapa* mapa, char* id);

/**
 * Esta função é responsável por liberar a memória alocada para a estrutura de dados Mapa.
 * 
 * @param mapa Ponteiro para a estrutura de dados Mapa
 * @return     0 se a destruição foi bem-sucedida. -1 caso contrário
 */
int destruirMapa(Mapa* mapa);

#endif