#include <stdio.h>
#include <stdlib.h>

#include "../src/geo.h"
#include "../src/params.h"
#include "../src/hashBin.h"
#include "../unity/unity.c"

// Variável global para ser usada nos testes
Param* param_teste = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes.
void setUp(void){
    freeParametros(param_teste);
    param_teste = NULL;

    remove("teste.hf");
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    freeParametros(param_teste);
    param_teste = NULL;

    remove("teste.hf");
}

// 1: Testa se a função montarCaminhoGEO realmente monta o caminho completo do arquivo .geo e retorna 0
void test_MontarCaminhoGEO_DeveRetornarZero(){
    // 1.1: Cria uma estrutura de parâmetros para ser usada no teste de montagem do caminho do arquivo .geo
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeGeo(param_teste, "t1.geo");

    // 1.2: Buffer para armazenar o caminho completo do arquivo .geo montado pela função montarCaminhoGeo 
    char caminhoGeo[512];

    // 1.3: Tenta montar o caminho completo do arquivo .geo usando a função montarCaminhoGeo 
    // e verifica se a função retornar 0 (sucesso)
    int resultado = montarCaminhoGeo(param_teste, caminhoGeo);
    TEST_ASSERT_EQUAL_INT(0, resultado);

    // 1.4: Verifica se o caminho completo do arquivo .geo montado pela função montarCaminhoGeo corresponde ao esperado, 
    // garantindo que a função monte o caminho corretamente
    TEST_ASSERT_EQUAL_STRING("./t1.geo", caminhoGeo);
}

// 2: Testa se a função readFileGeo realmente lê o arquivo .geo e retorna 0
void test_LerArquivoGEO_DeveRetornarZero(){
    // 2.1: Cria uma estrutura de parâmetros para ser usada no teste de leitura do arquivo .geo
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeGeo(param_teste, "t1.geo");

    // 2.2: Buffer para armazenar o caminho completo do arquivo .geo montado pela função montarCaminhoGeo
    char caminhoGeo[512];
    montarCaminhoGeo(param_teste, caminhoGeo);

    // 2.3: Tenta abrir o arquivo .geo para leitura e verifica se o arquivo foi aberto com sucesso
    FILE* arquivoGeo = fopen(caminhoGeo, "r");
    TEST_ASSERT_NOT_NULL(arquivoGeo);

    // 2.4: Tenta ler o arquivo .geo usando a função readFileGeo e verifica se a função retornar 0 (sucesso)
    HashBin* h = criarHash("teste.hf");
    Quadras* q = criarQuadra();
    TEST_ASSERT_EQUAL_INT(0, readFileGeo(arquivoGeo, h, q, param_teste));

    // 2.5: Fecha o arquivo .geo após a leitura para liberar os recursos alocados durante o teste
    fclose(arquivoGeo);
}

// 3: Testa se a função processarGeo realmente processa o arquivo .geo e retorna 0
void test_ProcessarArquivoGEO_DeveRetornarZero(){
    // 3.1: Cria uma estrutura de parâmetros para ser usada no teste de processamento do arquivo .geo
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./");
    setNomeGeo(param_teste, "t1.geo");

    // 3.2: Tenta processar o arquivo .geo usando a função processarGeo e verifica se a função retornar 0 (sucesso)
    HashBin* h = criarHash("teste.hf");
    Quadras* q = criarQuadra();
    TEST_ASSERT_EQUAL_INT(0, processarGeo(param_teste, h, q));
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes do GEO
    printf("#================= INICIO DOS TESTES DO GEO ================#\n\n");
    RUN_TEST(test_MontarCaminhoGEO_DeveRetornarZero);
    RUN_TEST(test_LerArquivoGEO_DeveRetornarZero);
    RUN_TEST(test_ProcessarArquivoGEO_DeveRetornarZero);
    printf("\n#================== FIM DOS TESTES DO GEO ==================#");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}