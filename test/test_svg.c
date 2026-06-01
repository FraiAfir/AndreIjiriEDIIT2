#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/svg.h"
#include "../unity/unity.c"

// Declaração de variáveis globais para os testes
FILE* svg_teste = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes e
// garantindo a confiabilidade dos resultados dos testes.
void setUp(void){
    svg_teste = NULL;
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    if(svg_teste != NULL){
        fecharSvg(svg_teste);
        svg_teste = NULL;
    }
}

// 1: Testa se a função criarSvg retorna um ponteiro válido ao criar um arquivo .svg
void test_CriarSvg_DeveRetornarPonteiroValido(void){
    // 1.1: Chama a função criarSvg para criar um arquivo .svg de teste
    svg_teste = criarSvg("../test/teste.svg", 500.0, 500.0);

    // 1.2: Verifica se o ponteiro retornado pela função criarSvg é diferente de NULL, 
    // indicando que o arquivo .svg foi criado com sucesso
    TEST_ASSERT_NOT_NULL(svg_teste);
}

// 2: Testa se a função desenharFormaSvg retorna 0 ao desenhar uma forma geométrica no arquivo .svg
void test_DesenharFormaSvg_DeveRetornarZero(void){
    // 2.1: Chama a função criarSvg para criar um arquivo .svg de teste
    svg_teste = criarSvg("../test/teste.svg", 500.0, 500.0);

    // 2.2: Verifica se o ponteiro retornado pela função criarSvg é diferente de NULL, 
    // indicando que o arquivo .svg foi criado com sucesso
    TEST_ASSERT_NOT_NULL(svg_teste);

    // 2.3: Chama a função desenharFormaSvg para desenhar um retângulo no arquivo .svg de teste 
    // e verifica se o retorno é 0, indicando que a forma foi desenhada com sucesso
    int resultado = desenharFormaSvg(svg_teste, "r", 50.0, 50.0, 100.0, 100.0, "2.0px", "black", "red");
    TEST_ASSERT_EQUAL_INT(0, resultado);
}

// 3: Testa se a função fecharSvg retorna 0 ao fechar o arquivo .svg criado durante os testes
void test_FecharSvg_DeveRetornarZero(void){
    // 3.1: Chama a função criarSvg para criar um arquivo .svg de teste
    svg_teste = criarSvg("../test/teste.svg", 500.0, 500.0);

    // 3.2: Verifica se o ponteiro retornado pela função criarSvg é diferente de NULL, 
    // indicando que o arquivo .svg foi criado com sucesso
    TEST_ASSERT_NOT_NULL(svg_teste);

    // 3.3: Chama a função fecharSvg para fechar o arquivo .svg criado durante os testes 
    // e verifica se o retorno é 0, indicando que o arquivo foi fechado com sucesso
    int resultado = fecharSvg(svg_teste);
    TEST_ASSERT_EQUAL_INT(0, resultado);
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes do SVG
    printf("#=================== INICIO DOS TESTES DO SVG ===================#\n\n");
    RUN_TEST(test_CriarSvg_DeveRetornarPonteiroValido);
    RUN_TEST(test_DesenharFormaSvg_DeveRetornarZero);
    RUN_TEST(test_FecharSvg_DeveRetornarZero);
    printf("\n#==================== FIM DOS TESTES DO SVG =====================#");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}