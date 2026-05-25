# Makefile para o projeto TED - Trabalho de Estruturas de Dados
# Autor: André Felipe Ijiri Ribeiro

PROJ_NAME   = ted 		# Nome do executável a ser gerado para o projeto principal
TESTS_NAME  = ted_tests	# Nome do executável a ser gerado para os testes

ALUNO = Andre_Ijiri # Nome do aluno (sem espaços, para uso em nomes de arquivos)



# =========================  DETECÇÃO AUTOMÁTICA DO SISTEMA OPERACIONAL ========================= #
ifeq ($(OS),Windows_NT)
    # Comandos para Windows (CMD/PowerShell)
    RM   = del /Q /F
    EXEC = .exe
	LIBS = -lm -lssp
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
# -lssp: Biblioteca para proteção contra estouro de pilha (stack protector)
else
    # Comandos para Linux / macOS / WSL
    RM   = rm -f
    EXEC = 
	LIBS = -lm
# -lm:   Biblioteca matemática para funções como sqrt, pow, etc.
endif
# =============================================================================================== #





# ================================== ARQUIVOS-FONTE E OBJETOS =================================== #
# Compilador
CC = gcc
# Flags
CFLAGS  = -g -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall
LDFLAGS = 

# Arquivos-fonte e objetos
OBJETOS = main.o params.o
TESTS   = /test/test_params.c
SRCS    = params.c
# =============================================================================================== #





# ==================================== REGRAS DE COMPILAÇÃO ===================================== #
# Regra para compilar os arquivos de teste e gerar o executável de testes
$(TESTS_NAME): $(TESTS) $(SRCS)
	@echo "Compilando os arquivos de teste $(TESTS_NAME): ..."
	$(CC) -o $(TESTS_NAME) $(TESTS) $(SRCS) $(LIBS) $(LDFLAGS)
	@echo "Compilacao dos testes finalizada com sucesso."



# Regra padrão para compilar o projeto principal e gerar o executável
$(PROJ_NAME): $(OBJETOS)
	@echo "Compilando o projeto principal $(PROJ_NAME): ..."
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS) $(LDFLAGS)
	@echo "Compilacao finalizada com sucesso."

# Regra para compilar os arquivos .c em .o
%.o : %.c
	@echo "Compilando $< ..."
	$(CC) -c $(CFLAGS) $< -o $@
# =============================================================================================== #





# ================================ DEPENDÊNCIAS DOS ARQUIVOS .o ================================= #
# Dependências dos arquivos objetos 
# para garantir que sejam recompilados quando os arquivos de cabeçalho forem modificados
params.o: params.c params.h

main.o: main.c params.h
# =============================================================================================== #





# =======================================  CLEAN & PHONY ======================================== #
# Regra para limpar os arquivos objetos e o executável
clean:
	@echo "Limpando os arquivos objetos e o executavel..."
	$(RM) $(OBJETOS) $(PROJ_NAME)$(EXEC)

# phony para evitar conflitos com arquivos chamados "clean" ou "all" no diretório
.PHONY: clean
# =============================================================================================== #