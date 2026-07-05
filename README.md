# AndreIjiriEDIIT2
Repositória para o Trabalho 2 da matéria de Estrutura de Dados II

Aluno:     André Felipe Ijiri Ribeiro
Matrícula: 202400560032



# ATU (Arquivo de Teste Unitário)
C:\Users\andre\OneDrive\Documentos\GitHub\AndreIjiriEDIIT2
mingw32-make ted_tests



# Compilação
C:\Users\andre\OneDrive\Documentos\GitHub\AndreIjiriEDIIT2\src
mingw32-make
./ted -e ../entradas/ -f t1.geo -v t3.via -q consultas/t2.qry -o ../saidas/



# Clean
mingw32-make clean



# Valgrind
Abrir Powershell
Digitar "wsl"
Digitar "cd OneDrive/Documentos/GitHub/AndreIjiriEDIIT2/src"
Compilar com o Makefile: "make"
Executar "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ted -f ARQUIVO.geo -q ARQUIVO.qry -o ./"