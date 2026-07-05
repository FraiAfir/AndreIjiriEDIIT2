#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/qry.h"
#include "../unity/unity.c"

// Variável global para ser usada nos testes
HashBin* tabelaQ_teste = NULL;
Param*   param_teste   = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados dos testes.
void setUp(void){
    freeHash(tabelaQ_teste);
    freeParametros(param_teste);
    tabelaQ_teste = NULL;
    param_teste = NULL;

    remove("hashQ_teste.hfc");
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    freeHash(tabelaQ_teste);
    freeParametros(param_teste);
    tabelaQ_teste = NULL;
    param_teste = NULL;

    remove("hashQ_teste.hfc");
}

// 1: Testar se a função criarQry retorna um ponteiro válido (não NULL)
void test_CriarQry_DeveRetornarPonteiroValido(void){
    // 1.1: Chamar a função criarQry para criar um objeto de QRY
    Qry* qry = criarQry();

    // 1.2: Verificar se o ponteiro retornado é válido (não NULL)
    TEST_ASSERT_NOT_NULL(qry);

    // 1.3: Liberar o objeto de QRY criado para evitar vazamento de memória
    free(qry);
}

// 2: Testar se a função processarQry retorna 0 em caso de sucesso
void test_ProcessarQry_DeveRetornarZero(void){
    // 2.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t1.qry");

    // 2.2: Cria uma tabela hash de teste para quadras
    tabelaQ_teste = criarHash(param_teste);

    // 2.3: Popular as tabelas hash de teste com dados fictícios 
    // para garantir que a função processarQry tenha dados para processar durante o teste
    inserirReg(tabelaQ_teste, "q1", 10.0, 20.0, 30.0, 40.0, "1.0px", "red", "black");

    // 2.4: Chamar a função processarQry com os parâmetros e tabelas hash de teste
    int resultado = processarQry(param_teste, tabelaQ_teste);

    // 2.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
}

// 3: Testar se a função montarCaminhoQry retorna 0 em caso de sucesso
void test_MontarCaminhoQry_DeveRetornarZero(void){
    // 3.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t1.qry");
    char caminhoQry[256];

    // 3.2: Chamar a função montarCaminhoQry com os parâmetros de teste
    int resultado = montarCaminhoQry(param_teste, caminhoQry);

    // 3.3: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
}

// 3: Testar se a função readFileQry retorna 0 em caso de sucesso
void test_LerArquivoQry_DeveRetornarZero(void){
    // 3.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t1.qry");

    // 3.2: Buffer para armazenar o caminho completo do arquivo .qry montado pela função montarCaminhoQry
    char caminhoQry[256];
    montarCaminhoQry(param_teste, caminhoQry);

    // 3.2: Criar tabelas hash de teste para pessoas e quadras
    tabelaQ_teste = criarHash(param_teste);

    // 3.3: Abrir o arquivo .qry para leitura
    FILE* arquivoQry = fopen(caminhoQry, "r");

    // 3.4: Chamar a função readFileQry com os parâmetros e tabelas hash de teste
    int resultado = readFileQry(arquivoQry, tabelaQ_teste, param_teste);

    // 3.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);

    // 3.6: Fechar o arquivo .qry após o teste
    fclose(arquivoQry);
}

// 13: Testar se a função clonarSvgBase retorna um ponteiro válido (não NULL)
void test_ClonarSvgBase_DeveRetornarPonteiroValido(void){
    // 13.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t1.qry");
    setDirEntrada(param_teste, "./");
    setDirSaida(param_teste, "../test");

    // 13.2: Chamar a função clonarSvgBase para criar um objeto de SVG
    FILE* svg = clonarSvgBase("./t1.svg", "saida_clonar_svg_base_teste.svg");

    // 13.3: Verificar se o ponteiro retornado é válido (não NULL)
    TEST_ASSERT_NOT_NULL(svg);

    // 13.4: Fechar o arquivo de saída após o teste
    fclose(svg);
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes de QRY
    printf("#================================ INICIO DOS TESTES DE QRY ================================#\n\n");
    RUN_TEST(test_CriarQry_DeveRetornarPonteiroValido);
    RUN_TEST(test_ProcessarQry_DeveRetornarZero);
    RUN_TEST(test_MontarCaminhoQry_DeveRetornarZero);
    RUN_TEST(test_LerArquivoQry_DeveRetornarZero);
    RUN_TEST(test_ClonarSvgBase_DeveRetornarPonteiroValido);
    printf("\n#================================== FIM DOS TESTES DE QRY =================================#");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}