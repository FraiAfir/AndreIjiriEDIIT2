#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/via.h"
#include "../unity/unity.c"

// Variável global para ser usada nos testes
Grafo* grafo_teste = NULL;
Param* param_teste = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados dos testes.
void setUp(void){
    freeGrafo(grafo_teste);
    freeParametros(param_teste);
    grafo_teste = NULL;
    param_teste = NULL;

    remove("grafo_teste.via");
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    freeGrafo(grafo_teste);
    freeParametros(param_teste);
    grafo_teste = NULL;
    param_teste = NULL;

    remove("grafo_teste.via");
}

// 1: Testar se a função criarVia retorna um ponteiro válido (não NULL)
void test_CriarVia_DeveRetornarPonteiroValido(void){
    // 1.1: Chamar a função criarVia para criar um objeto de VIA
    Via* via = criarVia();

    // 1.2: Verificar se o ponteiro retornado é válido (não NULL)
    TEST_ASSERT_NOT_NULL(via);

    // 1.3: Liberar o objeto de VIA criado para evitar vazamento de memória
    free(via);
}

// 2: Testar se a função processarVia retorna 0 em caso de sucesso
void test_ProcessarVia_DeveRetornarZero(void){
    // 2.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeVia(param_teste, "t3.via");

    // 2.2: Cria uma tabela hash de teste para quadras
    grafo_teste = criarGrafo(2);

    // 2.3: Popular o grafo com vértices de teste 
    // para garantir que a função processarVia tenha dados para processar durante o teste
    inserirVertice(grafo_teste, "v1", 10.0, 20.0);
    inserirVertice(grafo_teste, "v2", 30.0, 40.0);

    // 2.4: Chamar a função processarVia com os parâmetros e o grafo de teste
    int resultado = processarVia(param_teste, grafo_teste);

    // 2.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
}

// 3: Testar se a função montarCaminhoVia retorna 0 em caso de sucesso
void test_MontarCaminhoVia_DeveRetornarZero(void){
    // 3.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeVia(param_teste, "t3.via");
    char caminhoVia[256];

    // 3.2: Chamar a função montarCaminhoVia com os parâmetros de teste
    int resultado = montarCaminhoVia(param_teste, caminhoVia);

    // 3.3: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
}

// 4: Testar se a função readFileVia retorna 0 em caso de sucesso
void test_LerArquivoVia_DeveRetornarZero(void){
    // 4.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeVia(param_teste, "t1.geo");

    // 4.2: Buffer para armazenar o caminho completo do arquivo .via montado pela função montarCaminhoVia
    char caminhoVia[256];
    montarCaminhoVia(param_teste, caminhoVia);

    // 4.2: Criar tabelas hash de teste para pessoas e quadras
    grafo_teste = criarGrafo(2);

    // 4.3: Abrir o arquivo .via para leitura
    FILE* arquivoVia = fopen(caminhoVia, "r");

    // 4.4: Chamar a função readFileVia com os parâmetros e tabelas hash de teste
    int resultado = readFileVia(arquivoVia, grafo_teste, param_teste);

    // 4.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);

    // 4.6: Fechar o arquivo .via após o teste
    fclose(arquivoVia);
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes de VIA
    printf("#================================ INICIO DOS TESTES DE VIA ================================#\n\n");
    RUN_TEST(test_CriarVia_DeveRetornarPonteiroValido);
    RUN_TEST(test_ProcessarVia_DeveRetornarZero);
    RUN_TEST(test_MontarCaminhoVia_DeveRetornarZero);
    RUN_TEST(test_LerArquivoVia_DeveRetornarZero);
    printf("\n#================================== FIM DOS TESTES DE VIA =================================#");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}