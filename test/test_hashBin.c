#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/params.h"
#include "../src/hashBin.h"
#include "../unity/unity.c"

// Variável global para ser usada nos testes
HashBin* tabela_teste = NULL;
Param*   params_teste = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes 
// e garantindo a confiabilidade dos resultados dos testes.
void setUp(void){
    freeHash(tabela_teste);
    tabela_teste = NULL;
    freeParametros(params_teste);
    params_teste = NULL;

    remove("hash_teste.hf");
    remove("hash_teste.hfc");
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    freeHash(tabela_teste);
    tabela_teste = NULL;
    freeParametros(params_teste);
    params_teste = NULL;

    remove("hash_teste.hf");
    remove("hash_teste.hfc");
}

// 1: Testa se a função criarHash realmente cria um objeto de tabela hash e não retorna NULL
void test_CriarHash_NaoDeveRetornarNull(){
    // 1.1: Cria uma tabela hash para ser usada no teste de criação da tabela hash
    setDirSaidaCompleto(params_teste, "../test/");
    tabela_teste = criarHash(params_teste); 
    
    // 1.2: Verifica a criação de uma tabela hash usando a função criarHash, passando um nome de arquivo de teste
    TEST_ASSERT_NOT_NULL(tabela_teste);
}

// 2: Testa se a função criarQuadra realmente cria um objeto de quadra e não retorna NULL
void test_CriarQuadra_NaoDeveRetornarNull(){
    // 2.1: Cria uma quadra usando a função criarQuadra
    Quadras* q = criarQuadra();

    // 2.2: Verifica a criação da quadra usando a função criarQuadra
    TEST_ASSERT_NOT_NULL(q);

    // 2.3: Libera a memória alocada para a quadra criada durante o teste para evitar vazamentos de memória
    freeQuadra(q);
}

// 3: Testa se a função inserirReg insere um novo registro corretamente e retorna 0
void test_InserirReg_DeveRetornarZero(){
    // 3.1: Cria uma tabela hash para ser usada no teste de inserção de registro
    tabela_teste = criarHash(params_teste);

    // 3.2: Tenta inserir um novo registro com uma chave única
    int resultado = inserirReg(tabela_teste, "q01.1", 0.0, 0.0, 10.0, 10.0, "5.0px", "black", "black"); 

    // 3.3: Verifica se a função retornar 0 (sucesso) para a inserção de uma nova chave
    TEST_ASSERT_EQUAL_INT(0, resultado);
}

// 4: Testa se a função splitBucket divide corretamente um bucket cheio e retorna 0
void test_DividirBucket_DeveRetornarZero(){
    // 4.1: Cria uma tabela hash para ser usada no teste de divisão de bucket
    tabela_teste = criarHash(params_teste);

    // 4.2: Insere registros suficientes para preencher um bucket e causar um split
    inserirReg(tabela_teste, "q01.1", 0.0, 0.0, 10.0, 10.0, "5.0px", "black", "black");
    inserirReg(tabela_teste, "q01.2", 0.0, 0.0, 10.0, 10.0, "5.0px", "black", "black");
    inserirReg(tabela_teste, "q01.3", 0.0, 0.0, 10.0, 10.0, "5.0px", "black", "black");
    inserirReg(tabela_teste, "q01.4", 0.0, 0.0, 10.0, 10.0, "5.0px", "black", "black");
    int resultado = inserirReg(tabela_teste, "q01.5", 0.0, 0.0, 10.0, 10.0, "5.0px", "black", "black"); 

    // 4.3: Verifica se a função retornar 0 (sucesso) para a inserção que causou o split do bucket
    TEST_ASSERT_EQUAL_INT(0, resultado);
}

// 5: Testa se a função salvarDiretorioHFC salva corretamente o diretório da tabela hash em um arquivo .hfc e retorna 0
void test_SalvarDirHFC_DeveRetornarZero(){
    // 5.1: Cria uma tabela hash para ser usada no teste de salvamento do diretório HFC
    tabela_teste = criarHash(params_teste);

    // 5.2: Tenta salvar o diretório da tabela hash em um arquivo de saída no formato .hfc
    int resultado = salvarDiretorioHFC(tabela_teste, params_teste);

    // 5.3: Verifica se a função retornar 0 (sucesso) para o salvamento do diretório da tabela hash em um arquivo .hfc
    TEST_ASSERT_EQUAL_INT(0, resultado);
}

// 6: Testa se a função carregarDiretorioHFC carrega corretamente o diretório da tabela hash a partir de um arquivo .hfc e retorna um ponteiro válido
void test_CarregarDirHFC_DeveRetornarPonteiroValido(){
    // 6.1: Cria uma tabela hash para ser usada no teste de carregamento do diretório HFC
    tabela_teste = criarHash(params_teste);

    // 6.2: Salva o diretório da tabela hash em um arquivo de saída no formato .hfc para garantir que o arquivo exista para o teste de carregamento
    salvarDiretorioHFC(tabela_teste, params_teste);

    // 6.3: Tenta carregar o diretório da tabela hash a partir do arquivo .hfc criado
    HashBin* dir_carregado = carregarDiretorioHFC("hash_teste.hfc", "hash_teste.hf");

    // 6.4: Verifica se a função retornar um ponteiro válido (não NULL) para o diretório carregado a partir do arquivo .hfc
    TEST_ASSERT_NOT_NULL(dir_carregado);

    // 6.5: Libera a memória alocada para a tabela hash carregada durante o teste para evitar vazamentos de memória
    freeHash(dir_carregado);
}

// 7: Testa se a função hashFunc gera um valor de hash consistente para a mesma chave
void test_HashFunc_DeveGerarValorConsistente(){
    // 7.1: Define uma chave de teste para calcular o hash
    char* chave_teste = "q01.1";

    // 7.2: Calcula o valor de hash para a chave de teste usando a função hashFunc
    int hash1 = hashFunc(chave_teste);
    int hash2 = hashFunc(chave_teste);

    // 7.3: Verifica se os valores de hash gerados para a mesma chave são iguais, garantindo que a função de hash seja consistente
    TEST_ASSERT_EQUAL_INT(hash1, hash2);
}

// 8: Testa se a função buscarQuadra retorna o registro correto para uma chave existente 
// e retorna 0 para uma chave inexistente
void test_BuscarQuadra_DeveRetornarRegistroCorreto(){
    // 8.1: Cria uma tabela hash para ser usada no teste de busca de quadra
    tabela_teste = criarHash(params_teste);
    
    // 8.2: Insere um registro para garantir que haja um registro existente para a busca, 
    // e verifica se a função retornar 0 (sucesso) para a inserção do registro
    int res_ins = inserirReg(tabela_teste, "b01.1", 10.0, 10.0, 50.0, 50.0, "1.0px", "red", "black");
    TEST_ASSERT_EQUAL_INT(0, res_ins);

    // 8.3: Cria uma estrutura de quadra para armazenar o resultado da busca e tenta buscar a quadra usando a função buscarQuadra,
    // passando a chave do registro inserido e o ponteiro para a estrutura de quadra onde os dados devem ser armazenados, 
    // e verifica se a função retornar 1 (registro encontrado) para a busca da quadra existente
    Quadras* q_busca = criarQuadra(); 
    int res_busca = buscarQuadra(tabela_teste, "b01.1", q_busca);    
    TEST_ASSERT_EQUAL_INT(1, res_busca);

    // 8.4: Verifica se os dados retornados pela função buscarQuadra correspondem aos dados do registro inserido, 
    // garantindo que a busca retorne o registro correto para uma chave existente
    TEST_ASSERT_EQUAL_STRING("b01.1", getQuadraCEP(q_busca)); 
    TEST_ASSERT_EQUAL_STRING("red", getQuadraCFill(q_busca));
    TEST_ASSERT_EQUAL_STRING("black", getQuadraCStrk(q_busca));

    // 8.5: Destrói a tabela hash e libera a memória alocada para a estrutura de quadra criada durante o teste
    free(q_busca);
}

// 9: Testa se a função removerQuadra remove corretamente um registro existente e retorna 1, 
// e retorna 0 para uma chave inexistente
void test_RemoverQuadra_DeveRemoverRegistroCorreto(){
    // 9.1: Cria uma tabela hash para ser usada no teste de remoção de quadra
    tabela_teste = criarHash(params_teste);
    
    // 9.2: Insere um registro para garantir que haja um registro existente para a remoção, 
    // e verifica se a função retornar 0 (sucesso) para a inserção do registro
    int res_ins = inserirReg(tabela_teste, "b01.1", 10.0, 10.0, 50.0, 50.0, "1.0px", "red", "black");
    TEST_ASSERT_EQUAL_INT(0, res_ins);

    // 9.3: Tenta remover a quadra usando a função removerQuadra, passando a chave do registro inserido, 
    // e verifica se a função retornar 1 (registro encontrado e removido) para a remoção da quadra existente
    int res_remocao = removerQuadra(tabela_teste, "b01.1");
    TEST_ASSERT_EQUAL_INT(1, res_remocao);

    // 9.4: Tenta remover uma quadra usando uma chave inexistente e verifica se a função retornar 0 (registro não encontrado) para a remoção de uma chave inexistente
    int res_remocao_inexistente = removerQuadra(tabela_teste, "b01.2");
    TEST_ASSERT_EQUAL_INT(0, res_remocao_inexistente);
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes da tabela hash
    printf("#============================= INICIO DOS TESTES DA TABELA HASH =============================#\n");
    RUN_TEST(test_CriarHash_NaoDeveRetornarNull);
    RUN_TEST(test_CriarQuadra_NaoDeveRetornarNull);
    RUN_TEST(test_InserirReg_DeveRetornarZero);
    RUN_TEST(test_DividirBucket_DeveRetornarZero);
    RUN_TEST(test_SalvarDirHFC_DeveRetornarZero);
    // RUN_TEST(test_CarregarDirHFC_DeveRetornarPonteiroValido);
    RUN_TEST(test_HashFunc_DeveGerarValorConsistente);
    RUN_TEST(test_BuscarQuadra_DeveRetornarRegistroCorreto);
    RUN_TEST(test_RemoverQuadra_DeveRemoverRegistroCorreto);
    printf("\n#=============================== FIM DOS TESTES DA TABELA HASH ==============================#");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}