# AndreIjiriEDIIT2
Repositória para o Trabalho 2 da matéria de Estrutura de Dados II

Aluno:     André Felipe Ijiri Ribeiro
Matrícula: 202400560032

# ATU (Arquivo de Teste Unitário)
C:\Users\andre\OneDrive\Documentos\GitHub\AndreIjiriEDIIT2
gcc ./test/test_ARQUIVO.c ./src/ARQUIVO.c ./unity/unity.c -I./src -I./unity -o ./test/test_runner
./test/test_runner

# Compilação
C:\Users\andre\OneDrive\Documentos\GitHub\AndreIjiriEDIIT2\src
mingw32-make
./ted -f ARQUIVO.geo -q ARQUIVO.qry ./
mingw32-make clean

# Valgrind
Abrir Powershell
Digitar "wsl"
Digitar "cd OneDrive/Documentos/GitHub/AndreIjiriEDIIT2/src"
Compilar com o Makefile: "make"
Executar "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ted -f ARQUIVO.geo -q ARQUIVO.qry -o ./"