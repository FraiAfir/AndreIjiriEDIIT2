#include <stdio.h>
#include <stdlib.h>

#include "../unity/unity.c"
#include "../src/grafo.h"

// Variável global para ser usada nos testes
Grafo* grafo_teste = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes 
// e garantindo a confiabilidade dos resultados dos testes.
void setUp(void){
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
}

// 1: Testa se a função criarGrafo realmente cria um objeto de grafo e não retorna NULL,
// e se a função freeGrafo libera a memória alocada para o grafo sem erros
void test_CriarGrafo_DeveRetornarPonteiroValido_E_FreeGrafo_DeveRetornarZero(void){
    // 1.1: Cria um grafo para ser usado no teste de criação e destruição do grafo
    grafo_teste = criarGrafo(5);
    // 1.2: Verifica a criação do grafo usando a função criarGrafo, passando um número máximo de vértices
    TEST_ASSERT_NOT_NULL(grafo_teste);

    // 1.4: Libera a memória alocada para o grafo criado durante o teste usando a função freeGrafo, e verifica se a função retornar 0 (sucesso) para a liberação do grafo
    int res_free = freeGrafo(grafo_teste);
    TEST_ASSERT_EQUAL_INT(0, res_free);
}

// 2: Testa se a função inserirVertice insere um novo vértice no grafo corretamente,
// e se a função getIndiceVertice retorna o índice correto para um vértice existente (NULL para um vértice inexistente)
// e se a função getNumVertices retorna o número correto de vértices no grafo após as inserções
void test_InserirVertices_DeveRetornarZero_E_GetIndiceVertice_DeveRetornarIndiceCorreto(void){
    int res_insercao = 0;

    // 2.1: Cria um grafo para ser usado no teste de inserção de vértices e busca de índices
    grafo_teste = criarGrafo(2);
    
    // 2.2: Insere dois vértices no grafo usando a função inserirVertice, 
    // e verifica se a função retorna 0 (sucesso) para cada inserção
    res_insercao = inserirVertice(grafo_teste, "v1", 10.0, 20.0);
    TEST_ASSERT_EQUAL_INT(0, res_insercao);
    res_insercao = inserirVertice(grafo_teste, "v2", -50.5, 30.2);
    TEST_ASSERT_EQUAL_INT(0, res_insercao);
    
    // 2.3: Verifica se os IDs textuais retornam os índices sequenciais corretos (0 e 1)
    TEST_ASSERT_EQUAL_STRING("v1", getIndiceVertice(grafo_teste, "v1"));
    TEST_ASSERT_EQUAL_STRING("v2", getIndiceVertice(grafo_teste, "v2"));
    
    // 2.4: Verifica se buscar um ID inexistente retorna NULL com segurança
    TEST_ASSERT_NULL(getIndiceVertice(grafo_teste, "v3"));

    // 2.5: Verifica se o número total de vértices do grafo está correto
    TEST_ASSERT_EQUAL_INT(2, getNumVertices(grafo_teste));
    
    // 2.6: Libera a memória alocada para o grafo criado durante o teste para evitar vazamentos de memória
    freeGrafo(grafo_teste);
}

// 3: Testa se a função inserirVertice lida corretamente com o estouro da capacidade máxima de vértices do grafo
void test_EstouroDeCapacidadeDeVertices(void){
    int res_insercao = 0;

    // 3.1: Cria um grafo com capacidade máxima de 2 vértices para ser usado no teste de estouro de capacidade de vértices
    grafo_teste = criarGrafo(2);

    // 3.2: Insere três vértices no grafo usando a função inserirVertice, 
    // e verifica se a função retorna -1 (erro) para a inserção do terceiro vértice, 
    // indicando que o grafo atingiu sua capacidade máxima de vértices
    inserirVertice(grafo_teste, "v1", 0.0, 0.0);
    inserirVertice(grafo_teste, "v2", 1.0, 1.0);
    res_insercao = inserirVertice(grafo_teste, "v3", 2.0, 2.0);
    TEST_ASSERT_EQUAL_INT(-1, res_insercao);
    
    // 3.3: Libera a memória alocada para o grafo criado durante o teste para evitar vazamentos de memória
    freeGrafo(grafo_teste);
}

// 4: Testa se a função inserirAresta insere uma aresta corretamente entre vértices existentes no grafo,
// e se a função lida corretamente com tentativas de inserir arestas entre vértices inexistentes, sem causar erros ou travamentos
void test_InserirArestas_DeveRetornarZero_E_TratamentoDeErros(void){
    int res_insercao = 0;

    // 4.1: Cria um grafo para ser usado no teste de inserção de arestas e tratamento de erros
    grafo_teste = criarGrafo(2);
    
    // 4.2: Insere dois vértices no grafo para serem usados na inserção de arestas
    inserirVertice(grafo_teste, "v1", 0.0, 0.0);
    inserirVertice(grafo_teste, "v2", 100.0, 0.0);
    
    // 4.3 - Inserção Válida: Conecta v1 -> v2 (Não deve estourar erro nem travar)
    res_insercao = inserirAresta(grafo_teste, "v1", "v2", "Londrina", "Cambé", 150.75, 60.0, "Av. Central");
    TEST_ASSERT_EQUAL_INT(0, res_insercao);
    
    // 4.4 - Inserção Inválida: Origem ou Destino não existem no Grafo (Deve retornar -1)
    res_insercao = inserirAresta(grafo_teste, "v1", "v_fantasma", "Londrina", "Cambé", 10.0, 40.0, "Rua Inexistente");
    TEST_ASSERT_EQUAL_INT(-1, res_insercao);
    res_insercao = inserirAresta(grafo_teste, "v_fantasma", "v2", "Londrina", "Cambé", 10.0, 40.0, "Rua Inexistente");
    TEST_ASSERT_EQUAL_INT(-1, res_insercao);

    // 4.5: Libera a memória alocada para o grafo criado durante o teste para evitar vazamentos de memória
    freeGrafo(grafo_teste);
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void) {
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes do grafo
    printf("#============================== INICIO DOS TESTES DO GRAFO ==============================#\n");
    RUN_TEST(test_CriarGrafo_DeveRetornarPonteiroValido_E_FreeGrafo_DeveRetornarZero);
    RUN_TEST(test_InserirVertices_DeveRetornarZero_E_GetIndiceVertice_DeveRetornarIndiceCorreto);
    RUN_TEST(test_EstouroDeCapacidadeDeVertices);
    RUN_TEST(test_InserirArestas_DeveRetornarZero_E_TratamentoDeErros);
    printf("\n#================================ FIM DOS TESTES DO GRAFO ===============================#");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}