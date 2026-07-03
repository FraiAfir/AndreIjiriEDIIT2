#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashBin.h"

#define TAM_STRING 32
#define TAM_BUCKET 5

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct quadras{
    char   cep[TAM_STRING];
    double x, y, w, h;
    char   sw[TAM_STRING];
    char   cfill[TAM_STRING];
    char   cstrk[TAM_STRING];
}Quadras;
typedef struct bucket{
    int     prof_local;         // Profundidade Local do Bucket
    int     qntd_regs;          // Quantidade de Registros atualmente armazenados no bucket
    Quadras regs[TAM_BUCKET];   // Array de Registros do tipo Quadras. Cada bucket pode armazenar até 5 registros (TAM_BUCKET)
}Bucket;
typedef struct hashBin{
    int   prof_global;          // Profundidade Global da Tabela Hash
    int   tam_dir;              // Tamanho do Diretório (Número de Buckets) = 2^Profundidade Global
    long* endr_disco;           // Array de endereços dos buckets no arquivo físico da tabela hash
    FILE* arq_hf;               // Ponteiro para o arquivo físico da tabela hash
}HashBin;
/*###############################################################################################*/



/************************************** FUNÇÕES AUXILIARES ***************************************/
/**
 * Função de hash para calcular o valor hash de uma string (CEP) usando o algoritmo DJB2, criado por Daniel J. Bernstein.
 * 
 * O valor inicial recomendado para a função de hash DJB2 é 5381, 
 * que é um número primo que ajuda a distribuir as chaves de forma mais uniforme na tabela hash.
 * 
 * A função itera sobre cada caractere da string, multiplicando o hash atual por 33 e somando o valor ASCII do caractere.
 * A escolha do número 33 é baseada em pesquisas que mostram que ele é um número primo que ajuda a distribuir as chaves de forma mais uniforme, 
 * reduzindo a ocorrência de colisões na tabela hash.
 */
unsigned int hashFunc(char* key){
    // 1: Inicializa o valor do hash com o valor inicial recomendado para a função de hash DJB2
    unsigned long hash = 5381;

    // 2: Itera sobre cada caractere da string, multiplicando o hash atual por 33 e somando o valor ASCII do caractere
    int c; 
    while((c = *key++)){
        /**
         * Multiplica o hash atual por 33 e soma o valor ASCII da letra
         * (hash << 5) + hash é equivalente a hash * 33, mas é mais eficiente em termos de operações de bitwise.
         * O número 33 é escolhido porque é um número primo que ajuda a distribuir as chaves
         */
        hash = ((hash << 5) + hash) + c;
    }

    // 3: Aplica uma série de operações de mistura (bitwise) para melhorar a distribuição dos bits do hash e reduzir a ocorrência de colisões
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;

    // 4: Retorna o valor do hash calculado para a string de entrada
    return hash;
}

int duplicarDiretorio(HashBin* dir, int indice_dir, Bucket bucket_antigo){
    // 1: Verifica se a profundidade local do bucket causador da colisão é igual à profundidade global da tabela hash
    if(bucket_antigo.prof_local == dir->prof_global){
        int tam_antigo = dir->tam_dir;      // Tamanho do diretório antes da duplicação (Número de buckets antes da duplicação)
        int tam_novo   = tam_antigo * 2;    // Tamanho do diretório após a duplicação (Número de buckets após a duplicação)

        // 1.1: Duplica o tamanho do vetor de endereços do diretório na RAM
        dir->endr_disco = (long*)realloc(dir->endr_disco, tam_novo * sizeof(long));
        if(dir->endr_disco == NULL){
            fprintf(stderr, "ERRO: Falha na realocacao do diretorio da tabela hash durante o slipBucket.\n");
            return -1;
        }

        // 1.2: Espelha os endereços dos buckets antigos para os novos índices do diretório
        // Exemplo: Se o diretório antigo tinha 2 buckets (Índices 0 e 1), após a duplicação, 
        // o novo diretório terá 4 buckets (Índices 0, 1, 2 e 3).
        // O bucket do índice 0 será espelhado para o índice 2, e o bucket do índice 1 será espelhado para o índice 3.
        for(int i = 0; i < tam_antigo; i++){
            dir->endr_disco[i + tam_antigo] = dir->endr_disco[i];
            // Exemplo: dir->endr_disco[2] = dir->endr_disco[0];
            // Exemplo: dir->endr_disco[3] = dir->endr_disco[1];
        }

        // 1.3: Atualiza a Profundidade Global e o Tamanho do Diretório
        dir->tam_dir = tam_novo; // Atualiza o tamanho do diretório para o novo valor (Número de buckets após a duplicação)
        dir->prof_global++;      // Aumenta a profundidade global em 1, pois agora estamos usando mais um bit para calcular o índice do bucket
    }

    return 0;
}

int redistribuirRegistros(HashBin* dir, int indice_dir, Bucket* bucket_antigo, Bucket* bucket_novo, int bit_divisor){
    // 1: Colocamos os 5 registros antigos num buffer temporário
    Quadras buffer[TAM_BUCKET];
    for (int i = 0; i < TAM_BUCKET; i++) buffer[i] = bucket_antigo->regs[i];
    
    // 2: Esvazia o bucket antigo para preencher de novo com os registros corretos
    bucket_antigo->qntd_regs = 0;
    
    // 3: Redistribuir os 5 registros
    for(int i = 0; i < TAM_BUCKET; i++){
        // 3.1: Calcula o índice do bucket para o registro atual usando a função de hash e o bit divisor
        unsigned int h = hashFunc(buffer[i].cep);
        
        // 3.2: Verifica o bit divisor para determinar se o registro deve permanecer no bucket antigo ou ser movido para o novo bucket
        if((h & bit_divisor) == 0) bucket_antigo->regs[bucket_antigo->qntd_regs++] = buffer[i];
        else                       bucket_novo->regs[bucket_novo->qntd_regs++]     = buffer[i];
    }

    return 0;
}

int atualizarDiretorio(HashBin* dir, long offset_bucket_antigo, long offset_bucket_novo, Bucket* bucket_antigo, Bucket* bucket_novo, int bit_divisor){
    // 1: Procura no diretório todos os ponteiros que apontavam para o bucket_antigo 
    // e que possuem o 'bit_divisor' igual a 1, e muda eles para o bucket_novo
    for(int i = 0; i < dir->tam_dir; i++){
        if(dir->endr_disco[i] == offset_bucket_antigo)
            if((i & bit_divisor) != 0) dir->endr_disco[i] = offset_bucket_novo;
    }

    // 2: Salva os dois buckets atualizados fisicamente no HD
    fseek(dir->arq_hf, offset_bucket_antigo, SEEK_SET);
    fwrite(bucket_antigo, sizeof(Bucket), 1, dir->arq_hf);
    
    fseek(dir->arq_hf, offset_bucket_novo, SEEK_SET);
    fwrite(bucket_novo, sizeof(Bucket), 1, dir->arq_hf);

    return 0;
}

int buscarQuadra(HashBin* dir, char* cep, Quadras* resultado){
    // 1: Calcular o Hash e o Índice no diretório
    int valor_hash = hashFunc(cep);

    // 2: Aplicamos a máscara para pegar apenas os bits da prof_global atual
    int indice = valor_hash & ((1 << dir->prof_global) - 1); 
    /**
     * Calcula o índice do diretório usando os últimos 'p' bits do hash do CEP, onde 'p' é a profundidade global da tabela hash. 
     * A expressão (1 << dir->prof_global) - 1 cria uma máscara que tem os últimos 'p' bits definidos como 1 e os demais como 0, 
     * permitindo que apenas os últimos 'p' bits do valor_hash sejam usados para calcular o índice do diretório. 
     * Isso é essencial para garantir que o índice seja calculado corretamente com base na profundidade global da tabela hash, 
     * especialmente após operações de split que podem aumentar a profundidade global.
     */
    
    // 3: Pegar o offset no disco
    long offset = dir->endr_disco[indice];

    // 4: Ler o Bucket do disco
    Bucket b;
    fseek(dir->arq_hf, offset, SEEK_SET);
    fread(&b, sizeof(Bucket), 1, dir->arq_hf);

    // 5: Procurar o CEP dentro do balde
    for(int i = 0; i < b.qntd_regs; i++){
        if(strcmp(b.regs[i].cep, cep) == 0){
            *resultado = b.regs[i]; // Copia os dados para o retorno
            return 1;
        }
    }

    return 0;
}

int removerQuadra(HashBin* dir, char* cep){
    // 1: Calcular o Hash e o Índice no diretório
    int valor_hash = hashFunc(cep);
    int indice = valor_hash & ((1 << dir->prof_global) - 1);

    // 2: Atribui o offset do bucket correspondente ao índice do diretório para acessar o bucket correto no disco
    long offset = dir->endr_disco[indice];

    // 3: Declara estrutura para armazenar os dados do bucket lido do disco
    Bucket b; 

    // 4: Posiciona o ponteiro do arquivo no início do bucket correspondente ao índice do diretório
    fseek(dir->arq_hf, offset, SEEK_SET);

    // 5: Lê os dados do bucket do arquivo físico da tabela hash para a estrutura de dados do bucket na memória RAM
    fread(&b, sizeof(Bucket), 1, dir->arq_hf);

    // 6: Procura pelo CEP no bucket atual. Se encontrar, remove a quadra do bucket e salva o bucket atualizado no disco.
    for(int i = 0; i < b.qntd_regs; i++){
        // 6.1: Encontrou a quadra. Agora remove ela do bucket.
        if(strcmp(b.regs[i].cep, cep) == 0){
            // 6.1.1: Substitui o registro da quadra a ser removida pelo último registro do bucket.
            b.regs[i] = b.regs[b.qntd_regs - 1];    
            /**
             * Isso é feito para evitar "buracos" no array de registros do bucket, 
             * mantendo os registros contíguos e facilitando a gestão do espaço no bucket
            */

            // 6.1.2: Diminui a quantidade de registros do bucket em 1, pois um registro foi removido 
            b.qntd_regs--;  
            /**
             * Isso é importante para manter o controle correto do número de registros atualmente armazenados no bucket, 
             * garantindo que as operações de inserção e remoção funcionem corretamente 
             * e que o bucket não seja considerado cheio quando na verdade tem espaço disponível após a remoção de um registro
            */

            // 6.1.3: Volta o ponteiro e sobrescreve o balde atualizado no disco
            fseek(dir->arq_hf, offset, SEEK_SET);
            fwrite(&b, sizeof(Bucket), 1, dir->arq_hf);
            return 1;
        }
    }

    // 7: Se não encontrar o CEP no bucket, retorna 0 para indicar que a quadra não foi encontrada para remoção
    return 0;
}

/**
 * Estas são as funções getters e setters para acessar e modificar os campos da estrutura Quadras,
 * permitindo a manipulação dos dados de uma quadra de forma encapsulada.
 * Cada função é responsável por acessar ou modificar um campo específico da estrutura Quadras,
 * como as coordenadas, dimensões, cores, etc., garantindo a integridade dos dados e facilitando a manutenção do código.
 */
double getQuadraX    (Quadras* q) { return q->x;     }
double getQuadraY    (Quadras* q) { return q->y;     }
double getQuadraW    (Quadras* q) { return q->w;     }
double getQuadraH    (Quadras* q) { return q->h;     }
char*  getQuadraSW   (Quadras* q) { return q->sw;    }
char*  getQuadraCEP  (Quadras* q) { return q->cep;   }
char*  getQuadraCStrk(Quadras* q) { return q->cstrk; }
char*  getQuadraCFill(Quadras* q) { return q->cfill; }
/*###############################################################################################*/



/*                                        FUNÇÕES PRINCIPAIS                                     */
HashBin* criarHash(Param* params){
    // 1: Aloca a estrutura do Diretório na memória RAM
    HashBin* dir = (HashBin*)malloc(sizeof(HashBin));
    if(dir == NULL){
        printf("ERRO: Falha na alocacao da tabela hash.\n");
        return NULL;
    }

    // 2: Cria/Abre o arquivo físico no disco no modo Binário de Leitura e Escrita
    // "wb+" Cria um arquivo novo limpo
    // "rb+" Abre um arquivo existente para leitura e escrita (mantendo o conteúdo existente)

    // 2.1: Monta o caminho de saída do arquivo físico da tabela hash usando o diretório de saída completo fornecido pelos parâmetros e o nome do arquivo "cidade.hf"
    char nomeArquivoFisico[256];                            // Buffer para armazenar o caminho completo do arquivo físico do diretório (.hf)
    strcpy(nomeArquivoFisico, getDirSaidaCompleto(params)); // nomeArquivoFisico = "./saida/"
    strcat(nomeArquivoFisico, "cidade.hf");                // "./saida/" + "cidade.hf" = "./saida/cidade.hf"

    dir->arq_hf = fopen(nomeArquivoFisico, "wb+");
    if(dir->arq_hf == NULL){
        printf("ERRO: Falha na abertura do arquivo físico da tabela hash.\n");
        free(dir);
        return NULL;
    }

    // 3: Inicializa as regras do Hashing Estendido (Profundidade 1 = 2 espaços)
    // 3.1: Inicializa a Profundidade Global e o Tamanho do Diretório
    dir->prof_global = 1; // Profundidade Global Inicial = 1
    dir->tam_dir     = 2; // Tamanho do Diretório Inicial = 2^Profundidade Global = 2^1 = 2
    // 3.2: Aloca o Diretório na RAM para armazenar os endereços dos buckets no arquivo
    dir->endr_disco  = (long*)malloc(dir->tam_dir * sizeof(long));
    if(dir->endr_disco == NULL){
        printf("ERRO: Falha na alocacao do diretorio da tabela hash.\n");
        fclose(dir->arq_hf);
        free(dir);
        return NULL;
    }

    // 4: Cria um Balde Vazio padrão na RAM para copiarmos pro disco
    Bucket bucket_vazio;
    bucket_vazio.prof_local = 1;
    bucket_vazio.qntd_regs  = 0;

    // 4.1: Zera toda a memória do array de registros para não gravar lixo do C no disco
    /**
     * memset é uma função da biblioteca string.h que preenche um bloco de memória com um valor específico.
     * memset(destino, valor, tamanho) -> Preenche o bloco de memória apontado por destino com o valor especificado,
     * por um número de bytes definido por tamanho.
    */
    memset(bucket_vazio.regs, 0, TAM_BUCKET * sizeof(Quadras));

    // 5: Gravando o Primeiro e o Segundo Balde (Índice 0 e 1) no disco
    /**
     * ftell é uma função da biblioteca stdio.h que retorna a posição atual do ponteiro de arquivo em bytes a partir do início do arquivo.
     * ftell(arquivo) -> Retorna a posição atual do ponteiro de arquivo em bytes a partir do início do arquivo.
     * fwrite é uma função da biblioteca stdio.h que escreve dados de um buffer para um arquivo.
     * fwrite(buffer, tamanho, quantidade, arquivo) -> Escreve dados do buffer para o arquivo,
     * onde tamanho é o tamanho em bytes de cada elemento a ser escrito, quantidade é o número de elementos a serem escritos,
     * e arquivo é o ponteiro para o arquivo onde os dados serão escritos.
    */
    // 5.1: Armazena o endereço do início do arquivo para o primeiro bucket e escreve o bucket vazio no arquivo
    dir->endr_disco[0] = ftell(dir->arq_hf);
    fwrite(&bucket_vazio, sizeof(Bucket), 1, dir->arq_hf);
    // 5.2: Armazena o endereço do início do arquivo para o segundo bucket e escreve o bucket vazio no arquivo
    dir->endr_disco[1] = ftell(dir->arq_hf);
    fwrite(&bucket_vazio, sizeof(Bucket), 1, dir->arq_hf);

    // 6: Retorna o ponteiro para a tabela hash criada
    return dir;
}

Quadras* criarQuadra(){
    // 1: Aloca a estrutura de Quadras na memória RAM
    Quadras* q = (Quadras*)malloc(sizeof(Quadras));
    if(q == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Quadras\n");
        return NULL;
    }

    // 2: Inicializa os campos da estrutura Quadras com valores padrão
    strcpy(q->cep, "");     // Inicializa o CEP como string vazia
    q->x = 0.0;             // Inicializa a coordenada x como 0.0
    q->y = 0.0;             // Inicializa a coordenada y como 0.0
    q->w = 0.0;             // Inicializa a largura como 0.0
    q->h = 0.0;             // Inicializa a altura como 0.0
    strcpy(q->sw, "1.0px"); // Inicializa a espessura da borda como 1.0 (valor padrão)
    strcpy(q->cfill, "");   // Inicializa a cor de preenchimento como string vazia
    strcpy(q->cstrk, "");   // Inicializa a cor da borda como string vazia

    // 3: Retorna o ponteiro para a quadra criada
    return q;
}

void freeHash(HashBin* dir){
    // 1: Verifica se o ponteiro para a tabela hash é nulo antes de tentar liberar a memória
    if(dir == NULL) return;

    // 2: Fecha o arquivo físico associado à tabela hash
    if(dir->arq_hf != NULL) fclose(dir->arq_hf);

    // 3: Libera a memória alocada para o Diretório da RAM
    if(dir->endr_disco != NULL) free(dir->endr_disco);

    // 4: Libera a memória alocada para a estrutura da tabela hash
    free(dir);
}

void freeQuadra(Quadras* q){
    // 1: Verifica se o ponteiro para a quadra é nulo antes de tentar liberar a memória
    if(q == NULL) return;

    // 2: Libera a memória alocada para a estrutura de quadras
    free(q);
}

int splitBucket(HashBin* dir, int indice_dir){
    // 1: Atribui o offset do bucket antigo a partir do diretório usando o índice do diretório onde ocorreu a colisão
    long offset_bucket_antigo = dir->endr_disco[indice_dir];

    // 2: Lê o bucket antigo do disco para a memória RAM para manipulação dos registros durante o processo de split
    Bucket bucket_antigo;
    fseek(dir->arq_hf, offset_bucket_antigo, SEEK_SET);
    fread(&bucket_antigo, sizeof(Bucket), 1, dir->arq_hf);
    
    // 3: Duplicar o diretório (Se necessário) | Aumentar a Profundidade Global e o Tamanho do Diretório
    if(duplicarDiretorio(dir, indice_dir, bucket_antigo) != 0){
        fprintf(stderr, "ERRO: Falha ao duplicar o diretorio durante o slipBucket.\n");
        return -1;
    }

    // 4: Criar um novo bucket vazio no final do arquivo para armazenar os registros que serão redistribuídos
    Bucket bucket_novo;
    memset(&bucket_novo, 0, sizeof(Bucket));
    bucket_novo.prof_local = bucket_antigo.prof_local + 1;

    // 5: Incrementa a profundidade de ambos os buckets (antigo e novo)
    bucket_antigo.prof_local++;
    bucket_novo.prof_local = bucket_antigo.prof_local;

    // 6: Busca a posição física do novo bucket
    fseek(dir->arq_hf, 0, SEEK_END);
    long offset_bucket_novo = ftell(dir->arq_hf);    
    int bit_divisor = 1 << (bucket_antigo.prof_local - 1);

    // 7: Redistribuir os registros do bucket antigo entre o bucket antigo e o novo bucket, de acordo com a nova profundidade local
    if(redistribuirRegistros(dir, indice_dir, &bucket_antigo, &bucket_novo, bit_divisor) != 0){
        fprintf(stderr, "ERRO: Falha ao redistribuir os registros durante o slipBucket.\n");
        return -1;
    }

    // 8: Atualizar o diretório para apontar para os buckets correto (antigo e novo) de acordo com a nova profundidade local
    if(atualizarDiretorio(dir, offset_bucket_antigo, offset_bucket_novo, &bucket_antigo, &bucket_novo, bit_divisor) != 0){
        fprintf(stderr, "ERRO: Falha ao atualizar o diretorio durante o slipBucket.\n");
        return -1;
    }

    return 0;
}

int inserirReg(HashBin* dir, char* cep, double x, double y, double w, double h, char* sw, char* cfill, char* cstrk){
    // 1: Cria um novo registro do tipo Quadras com os dados fornecidos
    Quadras novaQuadra;
    memset(&novaQuadra, 0, sizeof(Quadras));

    // 2: Atribui os valores fornecidos para a nova quadra
    strcpy(novaQuadra.cep, cep);        // Copia o CEP para a nova quadra
    novaQuadra.x = x;                   // Atribui a coordenada x
    novaQuadra.y = y;                   // Atribui a coordenada y
    novaQuadra.w = w;                   // Atribui a largura
    novaQuadra.h = h;                   // Atribui a altura
    strcpy(novaQuadra.sw, sw);          // Atribui a espessura da borda
    strcpy(novaQuadra.cfill, cfill);    // Copia a cor de preenchimento para a nova quadra
    strcpy(novaQuadra.cstrk, cstrk);    // Copia a cor da borda para a nova quadra

    // 3: Calcula o hash matemático do CEP
    unsigned int hash_val = hashFunc(novaQuadra.cep);

    // 4: Aplica a máscara para descobrir o índice do Diretório (olhando 'p' bits)
    // 4.1: Atribuímos à p, a profundidade global atual da tabela hash
    int p = dir->prof_global;
    // 4.2: Fórmula para obter os últimos 'p' bits do hash (equivalente a hash_val % (2^p))
    unsigned int ult_bits = (1 << p) - 1;
    // 4.3: Índice do Diretório onde a quadra deve ser inserida
    unsigned int indice_dir = hash_val & ult_bits;
    /**
     * Exemplo:
     * hash_val = 12345678 (em binário: 101111000110000101001110)
     * p = 3 (Profundidade Global = 3 bits)
     * ult_bits = (1 << 3) - 1 = 8 - 1 = 7 (em binário: 000000000000000000000111)
     * indice_dir = hash_val & ult_bits = 101111000110000101001110 & 000000000000000000000111 = 000000000000000000000110 (em decimal: 6)
     * & => É uma operação bitwise AND que compara cada bit de hash_val com o correspondente bit de ult_bits.
     * O resultado é o valor dos últimos 'p' bits de hash_val, que determina o índice do diretório onde a quadra deve ser inserida.
    */

    // 5: Consulta o Diretório na RAM para saber o endereço real do disco
    long offset = dir->endr_disco[indice_dir];

    // 6: Vai até o bloco no disco e carrega o Balde para a memória
    Bucket balde_atual;
    fseek(dir->arq_hf, offset, SEEK_SET);
    fread(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);

    // 7: Verifica se há espaço neste balde
    if(balde_atual.qntd_regs < TAM_BUCKET){
        // SUCESSO: O balde não está cheio
        // Copia a nova quadra para o primeiro espaço livre
        int pos = balde_atual.qntd_regs;    // Posição do próximo espaço livre no array de registros do bucket (inicialmente 0, depois 1, 2, etc.)
        balde_atual.regs[pos] = novaQuadra; // Copia a nova quadra para o primeiro espaço livre do array de registros do bucket
        balde_atual.qntd_regs++;            // Aumenta o contador de moradores do balde

        // IMPORTANTE: Volta o ponteiro do disco para o início deste balde e sobrescreve o balde atualizado no disco para salvar a nova quadra inserida no bucket
        fseek(dir->arq_hf, offset, SEEK_SET);
        fwrite(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);        
        return 0;        
    }
    // 8: Se o balde estiver cheio, precisamos fazer o SPLIT
    else{
        splitBucket(dir, indice_dir);
        // 8.1: Após o split, tentamos inserir a quadra novamente (recursivamente) para que ela seja inserida no bucket correto (antigo ou novo)
        return inserirReg(dir, cep, x, y, w, h, sw, cfill, cstrk);
    }
}

int salvarDiretorioHFC(HashBin* dir, Param* params){
    // 1: Salva a estrutura do diretório da RAM para um arquivo físico (.hfc) 
    // para que ela possa ser carregada posteriormente, mantendo a persistência dos dados do diretório da tabela hash

    // 1.1: Monta o caminho de saída com o nome do arquivo físico do diretório (.hfc) para garantir que ele seja criado corretamente no local desejado
    char nomeArquivoFisico[256];                            // Buffer para armazenar o caminho completo do arquivo físico do diretório (.hfc)
    strcpy(nomeArquivoFisico, getDirSaidaCompleto(params)); // nomeArquivoFisico = "./saida/"
    strcat(nomeArquivoFisico, "cidade.hfc");                // "./saida/" + "cidade.hfc" = "./saida/cidade.hfc"

    // 1.2: Abre o arquivo de diretório para escrita em modo binário
    FILE* f = fopen(nomeArquivoFisico, "wb");
    // 1.3: Verifica se o arquivo foi aberto corretamente
    if(f == NULL){
        printf("ERRO: Nao foi possivel criar o arquivo cidade.hfc\n");
        return -1;
    }

    // 2: Grava os dados (Profundidade e Tamanho)
    fwrite(&(dir->prof_global), sizeof(int), 1, f);
    fwrite(&(dir->tam_dir), sizeof(int), 1, f);

    // 3: Grava o vetor inteiro de uma só vez
    fwrite(dir->endr_disco, sizeof(long), dir->tam_dir, f);

    // 4: Fecha o arquivo e retorna sucesso
    fclose(f);

    return 0;
}

HashBin* carregarDiretorioHFC(char* nomeArquivoHFC, char* nomeArquivoHF){
    // 1: Abre o arquivo de diretório para leitura em modo binário
    FILE* f = fopen(nomeArquivoHFC, "rb");
    if(f == NULL){
        printf("ERRO: Nao foi possivel abrir o arquivo quadras.hfc\n");
        return NULL;
    }

    // 2: Aloca a estrutura do Diretório na memória RAM
    HashBin* dir = malloc(sizeof(HashBin));
    if(dir == NULL){
        printf("ERRO: Nao foi possivel alocar memoria para o diretorio\n");
        free(dir);
        return NULL;
    }
    
    // 3: Lê a profundidade global, o tamanho do diretório e os endereços dos buckets do arquivo para a estrutura na RAM
//  fread(&variavel_destino, tamanho_de_cada_elemento, quantidade_de_elementos, arquivo)
    fread(&(dir->prof_global), sizeof(int), 1, f);
    fread(&(dir->tam_dir), sizeof(int), 1, f);

    // 4: Aloca o vetor de endereços dos buckets na RAM
    dir->endr_disco = malloc(sizeof(long) * dir->tam_dir);
    if(dir->endr_disco == NULL){
        printf("ERRO: Nao foi possivel alocar memoria para o vetor de enderecos\n");
        free(dir->endr_disco);
        free(dir);
        return NULL;
    }

    // 5: Lê os endereços dos buckets do arquivo para o vetor de endereços na RAM
    fread(dir->endr_disco, sizeof(long), dir->tam_dir, f);
    
    // 6: Abre o arquivo de dados (.hf) que já existe
    dir->arq_hf = fopen(nomeArquivoHF, "rb+"); 

    // 7: Fecha o arquivo de diretório e retorna o ponteiro para a estrutura do diretório carregada na RAM
    fclose(f);
    return dir;
}
/*###############################################################################################*/